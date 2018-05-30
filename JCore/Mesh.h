#pragma once
#include <vector>
#include "Texture.h"
#include "Shader.h"

//��������
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

//���������Ϣ
struct VertexBone {
	unsigned int BoneIDs[4];
	float BoneWeights[4];

	VertexBone() {
		memset(&BoneIDs, 0, sizeof(BoneIDs));
		memset(&BoneWeights, 0, sizeof(BoneWeights));
	}

	void AddVertexBone(unsigned int boneID, float weight);
};

//������Ϣ
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<VertexBone> bones;
	std::vector<Texture> textures;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBone> bones, std::vector<Texture> textureus);
	~Mesh();

	void Draw(Shader shader);

	unsigned int getVAO();

	void deleteGraphicsRes();

private:
	unsigned int VAO;
	unsigned int VBOs[3];
	void setupMesh();
};

