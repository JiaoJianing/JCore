#pragma once
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

//Õ¯∏Ò–≈œ¢
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

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

