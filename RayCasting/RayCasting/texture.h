#pragma once
#ifndef TEXTURE
#define TEXTURE
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "reader.h"
#include "data.h"
void LoadTexture(GLuint& texture, GLuint& shaderProgram) {
	int width, height;

	glGenTextures(1, &texture);

	//�ȼ�������Ԫ 0Ĭ�ϱ�����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture);

	//����������������ά����(һ��Ҫ�����������������)
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, Cubesize, Cubesize, Cubesize, 0, GL_RED, GL_FLOAT, voxels);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//��һ������ ��Ŀ��
	//�ڶ������� �༶����ļ���
	//���������� �洢���
	//����
	//�������Ĳ���ʼ��0
	//�洢���ݸ�ʽ
	//��ʼ���ػ�������
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);

	//ͨ��sampler2D ����������� �����൱����shader��������ͼ 0��ʾtexture0
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);


}
#endif // !TEXTURE
