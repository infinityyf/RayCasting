#define GLEW_STATIC
//#define GLM_FORCE_LEFT_HANDED
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderTools.h"
#include "bufferTools.h"
#include "matrixs.h"
#include "data.h"
#include "texture.h"


GLuint VBO;
GLuint VAO;
GLuint texure;

GLuint shaderProgram;


GLfloat deltaTime = 0.0f; //当前帧到上一帧的时间差
GLfloat lastFrame = 0.0f;	//上一帧时间
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

int width, height;

const char *vertexShaderSource = "vert.vs";
const char *fragmentShaderSource = "frag.fs";


glm::mat4 model(1.0f);
glm::mat4 view(1.0f);
glm::mat4 proj(1.0f);


//记录相机的一系列参数
GLfloat aspect = 45.0f;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//记录光照信息
glm::vec3 LightColor = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);

//记录体元数据
float voxelcell[8];
glm::vec3 voxelPos[8];

//记录最终显示的点集
vector<glm::vec3> sphere;

//边到端点的映射关系
int edge2point[][2] = {
	{0,1},
	{1,2},
	{2,3},
	{0,3},
	{4,5},
	{5,6},
	{6,7},
	{4,7},
	{0,4},
	{1,5},
	{2,6},
	{3,7}
};

//插值得到每个三角面片的坐标
glm::vec3 interpolation(float point1,float point2,glm::vec3 position1,glm::vec3 position2) {
	glm::vec3 temp = position1 + (position2 - position1) * (1.0f - point1) / (point2-point1);
	return temp;
}

//先建立cubeindex 
int cubeindex = 0;
void prepare() {
	readData();
	voxels = (GLfloat*)malloc(sizeof(GLfloat) * Cubesize * Cubesize * Cubesize);
	vertices = (GLfloat*)malloc(sizeof(GLfloat) * Cubesize * Cubesize * Cubesize * 5);
	for (int i = 0; i < Cubesize * Cubesize * Cubesize; i++) {
		for (int j = 0; j < 5; j++) {
			vertices[i * 5 + j] = Alldata[i][j];
		}
	}
	for (int i = 0; i < Cubesize * Cubesize * Cubesize; i++) {
		voxels[i] = vertices[i * 5 + 3];
	}
	int bound = Cubesize-1;
	for (int x = 0; x < bound; x++) {
		for (int y = 0; y < bound; y++) {
			for (int z = 0; z < bound; z++) {
				int i = x * Cubesize * Cubesize + y * Cubesize + z;
				//对于每一个体元，生成cubeindex
				//为了符合与最后的表产生对应关系，这一必须对点的顺序进行统一
				cubeindex = 0;
				if (voxels[i] < 1.0f)cubeindex |= 1;
				if (voxels[i + 1] < 1.0f)cubeindex |= 2;
				if (voxels[i + Cubesize] < 1.0f)cubeindex |= 8;
				if (voxels[i + Cubesize + 1] < 1.0f)cubeindex |= 4;
				if (voxels[i + Cubesize * Cubesize] < 1.0f)cubeindex |= 16;
				if (voxels[i + Cubesize * Cubesize + 1] < 1.0f)cubeindex |= 32;
				if (voxels[i + Cubesize * Cubesize + Cubesize] < 1.0f)cubeindex |= 128;
				if (voxels[i + Cubesize * Cubesize + Cubesize + 1] < 1.0f)cubeindex |= 64;

				//记录体元数据
				voxelcell[0] = voxels[i];
				voxelcell[1] = voxels[i + 1];
				voxelcell[2] = voxels[i + Cubesize + 1];
				voxelcell[3] = voxels[i + Cubesize];
				voxelcell[4] = voxels[i + Cubesize * Cubesize];
				voxelcell[5] = voxels[i + Cubesize * Cubesize + 1];
				voxelcell[6] = voxels[i + Cubesize * Cubesize + Cubesize + 1];
				voxelcell[7] = voxels[i + Cubesize * Cubesize + Cubesize];
				//记录体元位置
				voxelPos[0] = glm::vec3(vertices[i * 5], vertices[i * 5 + 1], vertices[i * 5 + 2]);
				voxelPos[1] = glm::vec3(vertices[(i + 1) * 5], vertices[(i + 1) * 5 + 1], vertices[(i + 1) * 5 + 2]);
				voxelPos[2] = glm::vec3(vertices[(i + Cubesize + 1) * 5], vertices[(i + Cubesize + 1) * 5 + 1], vertices[(i + Cubesize + 1) * 5 + 2]);
				voxelPos[3] = glm::vec3(vertices[(i + Cubesize) * 5], vertices[(i + Cubesize) * 5 + 1], vertices[(i + Cubesize) * 5 + 2]);
				voxelPos[4] = glm::vec3(vertices[(i + Cubesize * Cubesize) * 5], vertices[(i + Cubesize * Cubesize) * 5 + 1], vertices[(i + Cubesize * Cubesize) * 5 + 2]);
				voxelPos[5] = glm::vec3(vertices[(i + Cubesize * Cubesize + 1) * 5], vertices[(i + Cubesize * Cubesize + 1) * 5 + 1], vertices[(i + Cubesize * Cubesize + 1) * 5 + 2]);
				voxelPos[6] = glm::vec3(vertices[(i + Cubesize * Cubesize + Cubesize + 1) * 5], vertices[(i + Cubesize * Cubesize + Cubesize + 1) * 5 + 1], vertices[(i + Cubesize * Cubesize + Cubesize + 1) * 5 + 2]);
				voxelPos[7] = glm::vec3(vertices[(i + Cubesize * Cubesize + Cubesize) * 5], vertices[(i + Cubesize * Cubesize + Cubesize) * 5 + 1], vertices[(i + Cubesize * Cubesize + Cubesize) * 5 + 2]);
				//cout << voxels[i] << endl;
				//cout << (int)cubeindex << endl;
				//判断交点在哪条边上
				for (int j = 0; j < 16; j++) {
					if (triTable[cubeindex][j] == -1)break;
					//对于每一条相交边都可以插值计算出等值面交点位置
					int index1 = edge2point[triTable[cubeindex][j]][0];
					int index2 = edge2point[triTable[cubeindex][j]][1];
					//cout << index1<<"," << index2 << endl;
					glm::vec3 position = interpolation(voxelcell[index1], voxelcell[index2], voxelPos[index1], voxelPos[index2]);
					sphere.push_back(position);
					//cout << position.x<<" "<< position.y << " " << position.z << endl;
				}
			}
		}
	}
	
	points = (GLfloat*)malloc(sizeof(GLfloat) * sphere.size()*3);
	for (int i=0,j = 0; i < sphere.size(); i++,j+=3) {
		points[j] = sphere[i].x;
		points[j + 1] = sphere[i].y;
		points[j + 2] = sphere[i].z;
	}

	
}

void key_callback(GLFWwindow *window,int key,int scancode,int action,int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	GLfloat cameraSpeed = 0.5f * deltaTime;
	if (key == GLFW_KEY_W)
		cameraPos += cameraSpeed * cameraFront;
	if (key == GLFW_KEY_S)
		cameraPos -= cameraSpeed * cameraFront;
	if (key == GLFW_KEY_A)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标的范围是从下往上的
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//限制俯仰角
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (aspect >= 1.0f && aspect <= 45.0f)
		aspect -= yoffset * 0.05F;
	if (aspect <= 1.0f)
		aspect = 1.0f;
	if (aspect >= 45.0f)
		aspect = 45.0f;

	
}

void renderScence() {

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	//计算view proj
	glm::mat4 model(1.0f);
	ModelMatrix(model);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	proj = glm::perspective(aspect, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	//这一行没用
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "inverse"), 1, GL_FALSE, glm::value_ptr(model));
	
	glm::vec3 camera = glm::vec3(cameraPos.x, -cameraPos.z, cameraPos.y);
	glUniform3fv(glGetUniformLocation(shaderProgram, "camera"), 1, glm::value_ptr(camera));
	//如果不这么写每次都需要对VBO进行设置
	glBindVertexArray(VAO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//从当前绑定的EBO中获取索引 由于绑定了VAO,而VAO又绑定了EBO所以不需要重复操作
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//第二个参数 绘制点的个数
	//第三个参数 索引的类型
	//第四个参数 偏移量



	glDrawArrays(GL_TRIANGLES, 0,36);
	//glDrawArrays(GL_TRIANGLES, 0, sphere.size()*2);
	//按照顶点的顺序构成三角形 而不是按照索引
	glBindVertexArray(0);
}

int main() {
	prepare();


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(600, 600, "learnOpengl", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "fail to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//设置为当前上下文
	glfwMakeContextCurrent(window);

	//使用glew 管理opengl 指针函数
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "fail to init glew" << std::endl;
		return -1;
	}

	//设置视口大小
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//隐藏鼠标 并捕捉位置
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//设置回调函数 键盘\鼠标位置\滚轮事件
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	

	//创建顶点数组对象，该操作将VBP和VAO相互绑定
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
		//创建顶点缓冲对象
		CreateVertexBuffer(&VBO,sphere.size());
		//创建索引缓冲对象
		//CreateIndexBuffer(&EBO);
		//此时VAO已经和VBO EBO 相互关联
	glBindVertexArray(0);//解绑




	//编译着色器
	GLuint vertexShader;
	GLuint fragmentShader;
	CompileShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
	CompileShader(fragmentShader,fragmentShaderSource,GL_FRAGMENT_SHADER);

	//构建着色器程序对象，并在渲染时激活
	CreateProgram(shaderProgram,vertexShader,fragmentShader);

	//compile ==> link
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//创建三维纹理
	LoadTexture(texure, shaderProgram);

	//设置
	glEnable(GL_DEPTH_TEST);




	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		renderScence();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
