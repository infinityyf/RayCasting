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


////ע��������Ҫ���г�ʼ��
//glm::mat4 trans(1.0f);
////������ܻ��� ��Ҫת��
//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//
//GLuint transformMatrix = glGetUniformLocation(shaderProgram, "transform");
//glUniformMatrix4fv(transformMatrix, 1, GL_FALSE, glm::value_ptr(trans));
////�ڶ������� ��ʾ���뼸������
////���������� ��ʾ�Ƿ���о����û� ���Ǿ�Ϊ���������Բ���
////���ĸ����� ת��Ϊglsl���Խ��ܵĸ�ʽ

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
	//����view ����
	//ת�����ӵ�����ϵ֮��



}

#endif // !MATRIX_H
