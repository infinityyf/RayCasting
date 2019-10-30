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

	//先激活纹理单元 0默认被激活
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture);

	//根据体数据生成三维纹理(一定要对纹理参数进行设置)
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, Cubesize, Cubesize, Cubesize, 0, GL_RED, GL_FLOAT, voxels);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//第一个参数 绑定目标
	//第二个参数 多级纹理的级别
	//第三个参数 存储风格
	//长宽
	//接下来的参数始终0
	//存储数据格式
	//初始加载基本级别
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);

	//通过sampler2D 传递纹理对象 这里相当于在shader中设置贴图 0表示texture0
	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);


}
#endif // !TEXTURE
