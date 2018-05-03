#include "stdafx.h"
#include "Mesh.h"



Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textureus)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textureus;

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
		//��������Ԫ
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

	//��������
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Mesh::getVAO()
{
	return VAO;
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

#pragma region ����VAO
	glBindVertexArray(VAO);

	//���㻺��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	//��������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//����
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//����
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	//����
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	//������
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
#pragma endregion
}
