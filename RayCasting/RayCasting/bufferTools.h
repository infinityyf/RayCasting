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
	//标准化设备坐标会变成屏幕空间坐标
	//屏幕空间坐标，又会输入到片段着色器中，进行逐像素的片段着色

	//使用顶点缓冲对象 可以一次性发送大量顶点到显存

	glGenBuffers(1, VBO);

	//此时使用对GL_ARRAY_BUFFER的操作都是对VBO的操作
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//复制数据
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Cubesize * Cubesize * Cubesize * 5, vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size*3, points, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 36, cubeData, GL_STATIC_DRAW);

	//GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//GL_DYNAMIC_DRAW：数据会被改变很多。
	//GL_STREAM_DRAW ：数据每次绘制时都会改变
	//相当于指定数据存放的区域

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	//第一个参数 对于shader 中location
	//第二个参数 表示顶点属性的大小
	//第三个参数 数据类型
	//第四个参数 是否标准化
	//第五个参数 步长
	//第六个参数 数据在缓冲中的偏移量

	glEnableVertexAttribArray(0);
	//以顶点偏移作为参数


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

//void CreateIndexBuffer(GLuint *EBO) {
//	//创建索引缓冲坐标
//	glGenBuffers(1, EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//}

#endif // !BUFFER_H
