#pragma once
#include <vector>
#include "Texture.h"
#include "Shader.h"

//顶点数据
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

//网格信息
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textureus);
	~Mesh();

	void Draw(Shader shader);

	unsigned int getVAO();

	void deleteGraphicsRes();

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

