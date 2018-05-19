#include "stdafx.h"
#include "SpherePrimitive.h"
#include "Texture.h"
#include "ResourceManager.h"

SpherePrimitive::SpherePrimitive()
{
	initRenderData();
}


SpherePrimitive::~SpherePrimitive()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void SpherePrimitive::Render(Shader shader)
{
	shader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture.GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture.GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SpecularTexture.GetID());
	shader.setInt("material.texture_diffuse1", 0);
	shader.setInt("material.texture_normal1", 1);
	shader.setInt("material.texture_specular1", 2);
	shader.setMatrix4("model", GetWorldTransform());
	shader.setVec3("g_Color", GetColor());
	shader.setInt("g_highLight", GetHighLight());
	shader.setVec3("g_highLightColor", GetHighLightColor());
	shader.setInt("nodeID", GetID());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SpherePrimitive::RenderSimple(Shader shader)
{
	shader.use();
	shader.setVec3("g_Color", GetColor());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SpherePrimitive::initRenderData()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	//生成顶点数据
	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; y++) {
		for (unsigned int x = 0; x <= X_SEGMENTS; x++) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			//float xPos = std::cos(xSegment * 2.0f * PI) * std::cos(ySegment * PI);
			//float yPos = std::sin(xSegment * 2.0f * PI);
			//float zPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			float xPos = std::sin(ySegment * PI) * std::cos(xSegment * 2.0f * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(ySegment * PI) * std::sin(xSegment * 2.0f * PI);

			Vertex v;
			v.position = glm::vec3(xPos, yPos, zPos);
			v.normal = glm::vec3(xPos, yPos, zPos);
			v.texCoord = glm::vec2(xSegment * 5, ySegment * 5);
			v.tangent = glm::normalize(glm::vec3(-zPos, 0, -xPos));
			v.bitangent = glm::normalize(glm::cross(v.normal, v.tangent));
			m_Vertices.push_back(v);
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; y++) {
		if (!oddRow) {
			for (int x = 0; x <= X_SEGMENTS; x++) {
				m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
				m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else {
			for (int x = X_SEGMENTS; x >= 0; x--) {
				m_Indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				m_Indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	glBindVertexArray(m_VAO);

	//顶点缓冲
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	//索引缓冲
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

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

	glBindVertexArray(0);
}
