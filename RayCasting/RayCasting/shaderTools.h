#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

void CompileShader(GLuint &shader, const char* vertexShaderSource, GLenum shaderType) {
	GLint success;
	GLchar infoLog[512];
	std::string source;

	//����ָ������
	const GLchar *ShaderSource[1];

	//���Դ���ַ���
	std::ifstream fstream(vertexShaderSource);
	std::string line;
	while (getline(fstream, line)) {
		source.append(line);
		source.append("\n");
		//std::cout << source << std::endl;
	}
	fstream.close();
	ShaderSource[0] = source.c_str();


	shader = glCreateShader(shaderType);

	//ע��������Ҫ���ݵ���ָ������飬��������ΪҪ֧�ֶ��shaderͬʱ����
	glShaderSource(shader, 1, ShaderSource, NULL);
	glCompileShader(shader);
	//�鿴�������
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "compile fail" << infoLog << std::endl;
	}
}

void CreateProgram(GLuint &shaderProgram, GLuint &VertexShader, GLuint &FragmentShader) {
	GLint success;
	GLchar infoLog[512];

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, VertexShader);
	glAttachShader(shaderProgram, FragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "link fail" << infoLog << std::endl;
	}
}



void SetUniform(GLuint &shaderProgram) {
	GLfloat time = glfwGetTime();
	GLfloat greenValue = (sin(time) / 2) + 0.5;
	GLint location = glGetUniformLocation(shaderProgram, "ourColor");

	//����˵��ʹ��program
	glUseProgram(shaderProgram);
	glUniform4f(location, 0, greenValue, 0, 0);
}

#endif // !SHADER_H
