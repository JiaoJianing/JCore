#pragma once
#include <glm.hpp>
//¶¥µãÊý¾Ý
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};