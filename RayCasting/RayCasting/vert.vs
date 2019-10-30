#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoord;

out vec3 coord3d;
out mat4 inverse4frag;

uniform mat4 model; //ת��Ϊ��������
uniform mat4 view; //ת��Ϊ�������
uniform mat4 projection;	//ת��ΪͶӰ����
uniform mat4 inverse;

void main()
{
    gl_Position =projection * view*model *vec4(position.x, position.y, position.z, 1.0);
	coord3d = position;
	inverse4frag = inverse;
}
