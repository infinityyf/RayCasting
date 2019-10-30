//#include <iostream>
//#include <fstream>
//#include <glm/glm.hpp>
//#include <glm/vec3.hpp>
//#include <glm/geometric.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "reader.h"
//typedef struct Sphere {
//	glm::vec3 pos;
//	float radius;
//}Sphere,*Sphere_ptr;
//
//int cubesize = 20;
//std::string file = "data";
//
//void voxelGenerate() {
//	std::ofstream fstream(file);
//	Sphere sphere = { glm::vec3(0.0f, 0.0f, 0.0f) ,1};
//	//设置起始位置和步长
//	float stripe = sphere.radius / (cubesize / 2);
//	glm::vec3 startPos = glm::vec3(-sphere.radius, -sphere.radius, -sphere.radius);
//	fstream << cubesize <<std::endl;
//	for (int x = 0; x <= cubesize; x++) {
//		for (int y = 0; y <= cubesize; y++) {
//			for (int z = 0; z <= cubesize; z++) {
//				glm::vec3 shift = glm::vec3(x*stripe,y*stripe,z*stripe);
//				glm::vec3 currentPos = startPos + shift;
//				float distance = glm::distance(currentPos, sphere.pos);
//				std::cout << currentPos.x<< currentPos.y<< currentPos.z << std::endl;
//				std::cout << sphere.pos.x << sphere.pos.y << sphere.pos.z << std::endl;
//				std::cout << distance << std::endl;
//				if (distance < 1.0f) distance = 0.5f;
//				else distance = 0.1f;
//				fstream<< currentPos.x <<":"<< currentPos.y << ":" << currentPos.z << ":"<< distance <<":"<<x<< std::endl;
//			}
//		}
//	}
//	fstream.close();
//}
//
//int main(){
//	voxelGenerate();
//	//readData();
//	return 0;
//}