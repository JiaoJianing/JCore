#include "stdafx.h"
#include "Mesh.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBone> bones, std::vector<Texture> textureus)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textureus;
	this->bones = bones;

	setupMesh();
}


Mesh::~Mesh()
{
}

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int reflectNr = 1;
	unsigned int normalNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		//激活纹理单元
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].getType();
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_reflect") {
			number = std::to_string(reflectNr++);
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].GetID());
	}
	glActiveTexture(GL_TEXTURE0);

	//绘制网格
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Mesh::getVAO()
{
	return VAO;
}

void Mesh::deleteGraphicsRes()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBOs);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBOs);

#pragma region 配置VAO
	glBindVertexArray(VAO);

	//顶点缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//纹理
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	//切线
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	//副切线
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	if (bones.size() > 0) {
		//骨骼动画信息
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexBone), (void*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBone), (void*)offsetof(VertexBone, BoneWeights));
	}

	//索引缓冲
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
#pragma endregion
}

void VertexBone::AddVertexBone(unsigned int boneID, float weight)
{
	for (unsigned int i = 0; i < 4; i++) {
		if (BoneWeights[i] == 0.0) {
			BoneIDs[i] = boneID;
			BoneWeights[i] = weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	//assert(0);
}
