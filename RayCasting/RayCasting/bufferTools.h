#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "reader.h"
#include "data.h"

void CreateVertexBuffer(GLuint *VBO,int size) {
	//��׼���豸���������Ļ�ռ�����
	//��Ļ�ռ����꣬�ֻ����뵽Ƭ����ɫ���У����������ص�Ƭ����ɫ

	//ʹ�ö��㻺����� ����һ���Է��ʹ������㵽�Դ�

	glGenBuffers(1, VBO);

	//��ʱʹ�ö�GL_ARRAY_BUFFER�Ĳ������Ƕ�VBO�Ĳ���
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//��������
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Cubesize * Cubesize * Cubesize * 5, vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size*3, points, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 36, cubeData, GL_STATIC_DRAW);

	//GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	//GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	//GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
	//�൱��ָ�����ݴ�ŵ�����

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	//��һ������ ����shader ��location
	//�ڶ������� ��ʾ�������ԵĴ�С
	//���������� ��������
	//���ĸ����� �Ƿ��׼��
	//��������� ����
	//���������� �����ڻ����е�ƫ����

	glEnableVertexAttribArray(0);
	//�Զ���ƫ����Ϊ����


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

//void CreateIndexBuffer(GLuint *EBO) {
//	//����������������
//	glGenBuffers(1, EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//}

#endif // !BUFFER_H
