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


GLfloat deltaTime = 0.0f; //��ǰ֡����һ֡��ʱ���
GLfloat lastFrame = 0.0f;	//��һ֡ʱ��
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

int width, height;

const char *vertexShaderSource = "vert.vs";
const char *fragmentShaderSource = "frag.fs";


glm::mat4 model(1.0f);
glm::mat4 view(1.0f);
glm::mat4 proj(1.0f);


//��¼�����һϵ�в���
GLfloat aspect = 45.0f;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//��¼������Ϣ
glm::vec3 LightColor = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);

//��¼��Ԫ����
float voxelcell[8];
glm::vec3 voxelPos[8];

//��¼������ʾ�ĵ㼯
vector<glm::vec3> sphere;

//�ߵ��˵��ӳ���ϵ
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

//��ֵ�õ�ÿ��������Ƭ������
glm::vec3 interpolation(float point1,float point2,glm::vec3 position1,glm::vec3 position2) {
	glm::vec3 temp = position1 + (position2 - position1) * (1.0f - point1) / (point2-point1);
	return temp;
}

//�Ƚ���cubeindex 
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
				//����ÿһ����Ԫ������cubeindex
				//Ϊ�˷��������ı������Ӧ��ϵ����һ����Ե��˳�����ͳһ
				cubeindex = 0;
				if (voxels[i] < 1.0f)cubeindex |= 1;
				if (voxels[i + 1] < 1.0f)cubeindex |= 2;
				if (voxels[i + Cubesize] < 1.0f)cubeindex |= 8;
				if (voxels[i + Cubesize + 1] < 1.0f)cubeindex |= 4;
				if (voxels[i + Cubesize * Cubesize] < 1.0f)cubeindex |= 16;
				if (voxels[i + Cubesize * Cubesize + 1] < 1.0f)cubeindex |= 32;
				if (voxels[i + Cubesize * Cubesize + Cubesize] < 1.0f)cubeindex |= 128;
				if (voxels[i + Cubesize * Cubesize + Cubesize + 1] < 1.0f)cubeindex |= 64;

				//��¼��Ԫ����
				voxelcell[0] = voxels[i];
				voxelcell[1] = voxels[i + 1];
				voxelcell[2] = voxels[i + Cubesize + 1];
				voxelcell[3] = voxels[i + Cubesize];
				voxelcell[4] = voxels[i + Cubesize * Cubesize];
				voxelcell[5] = voxels[i + Cubesize * Cubesize + 1];
				voxelcell[6] = voxels[i + Cubesize * Cubesize + Cubesize + 1];
				voxelcell[7] = voxels[i + Cubesize * Cubesize + Cubesize];
				//��¼��Ԫλ��
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
				//�жϽ�������������
				for (int j = 0; j < 16; j++) {
					if (triTable[cubeindex][j] == -1)break;
					//����ÿһ���ཻ�߶����Բ�ֵ�������ֵ�潻��λ��
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
	GLfloat yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy����ķ�Χ�Ǵ������ϵ�
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//���Ƹ�����
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

	//����view proj
	glm::mat4 model(1.0f);
	ModelMatrix(model);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	proj = glm::perspective(aspect, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
	//��һ��û��
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "inverse"), 1, GL_FALSE, glm::value_ptr(model));
	
	glm::vec3 camera = glm::vec3(cameraPos.x, -cameraPos.z, cameraPos.y);
	glUniform3fv(glGetUniformLocation(shaderProgram, "camera"), 1, glm::value_ptr(camera));
	//�������ôдÿ�ζ���Ҫ��VBO��������
	glBindVertexArray(VAO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//�ӵ�ǰ�󶨵�EBO�л�ȡ���� ���ڰ���VAO,��VAO�ְ���EBO���Բ���Ҫ�ظ�����
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//�ڶ������� ���Ƶ�ĸ���
	//���������� ����������
	//���ĸ����� ƫ����



	glDrawArrays(GL_TRIANGLES, 0,36);
	//glDrawArrays(GL_TRIANGLES, 0, sphere.size()*2);
	//���ն����˳�򹹳������� �����ǰ�������
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

	//����Ϊ��ǰ������
	glfwMakeContextCurrent(window);

	//ʹ��glew ����opengl ָ�뺯��
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "fail to init glew" << std::endl;
		return -1;
	}

	//�����ӿڴ�С
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	//������� ����׽λ��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//���ûص����� ����\���λ��\�����¼�
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	

	//��������������󣬸ò�����VBP��VAO�໥��
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
		//�������㻺�����
		CreateVertexBuffer(&VBO,sphere.size());
		//���������������
		//CreateIndexBuffer(&EBO);
		//��ʱVAO�Ѿ���VBO EBO �໥����
	glBindVertexArray(0);//���




	//������ɫ��
	GLuint vertexShader;
	GLuint fragmentShader;
	CompileShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);
	CompileShader(fragmentShader,fragmentShaderSource,GL_FRAGMENT_SHADER);

	//������ɫ��������󣬲�����Ⱦʱ����
	CreateProgram(shaderProgram,vertexShader,fragmentShader);

	//compile ==> link
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//������ά����
	LoadTexture(texure, shaderProgram);

	//����
	glEnable(GL_DEPTH_TEST);




	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		renderScence();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
