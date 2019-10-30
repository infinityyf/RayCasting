#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

void ModelMatrix(glm::mat4 &model) {
	//glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//return model;
}

void ViewMatrix(glm::mat4 &view) {
	//glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//return view;
}

void ProjectionMatrix(glm::mat4 &proj,int width,int height) {
	//glm::mat4 proj(1.0f);
	proj = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	//return proj;
}


////注意这里需要进行初始化
//glm::mat4 trans(1.0f);
////这里接受弧度 需要转化
//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//
//GLuint transformMatrix = glGetUniformLocation(shaderProgram, "transform");
//glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, glm::value_ptr(trans));
////第二个参数 表示传入几个矩阵
////第三个参数 表示是否进行矩阵置换 但是均为列主序所以不用
////第四个参数 转化为glsl可以接受的格式

void Camera(glm::mat4 &view) {

	//GLfloat radius = 10.0f;
	//GLfloat camX = sin(glfwGetTime()) * radius;
	//GLfloat camZ = cos(glfwGetTime()) * radius;

	glm::vec3 cameraPos(0.0f,0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
	//创建view 矩阵
	//转化到视点坐标系之中



}

#endif // !MATRIX_H
