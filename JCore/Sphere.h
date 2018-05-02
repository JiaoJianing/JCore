#pragma once
#include "Shader.h"
#include <vector>

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Draw(Shader shader);

private:
	unsigned int m_VAO, m_VBO, m_EBO;
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec2> m_Uvs;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned int> m_Indices;
};

