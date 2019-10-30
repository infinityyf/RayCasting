 #version 330 core
in vec3 coord3d;
in mat4 inverse4frag;

out vec4 color;

uniform sampler3D ourTexture;
uniform vec3 lightColor; //������
uniform vec3 lightPos;   //���λ��
uniform vec3 camera;
//uniform vec4 ourColor;
void main(){
	vec3 dir = coord3d-camera;
	dir = normalize(dir);
	vec4 colorTemp;
	
	//����Ͷ�����
	for(float i=0;i<1.0f;i=i+0.01f){
		vec3 pos = coord3d+dir*i+vec3(0.5f,0.5f,0.5f);
		if(pos.x>1.0f || pos.y>1.0f || pos.z>1.0f || pos.x<0.0f || pos.y<0.0f || pos.z<0.0f) break;
		colorTemp = texture(ourTexture,pos).rrrr;
		//color = 0.5*colorTemp;
		color = 0.99*color+(0.01)*colorTemp;
	}
	//color =vec4( coord3d+vec3(0.5f,0.5f,0.5f),1.0);
}