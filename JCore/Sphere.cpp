#include "stdafx.h"
#include "Sphere.h"
#include "Texture.h"
#include "ResourceManager.h"

Sphere::Sphere()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; y++) {
		for (unsigned int x = 0; x <= X_SEGMENTS; x++) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			m_Positions.push_back(glm::vec3(xPos, yPos, zPos));
			m_Uvs.push_back(glm::vec2(xSegment, ySegment));
			m_Normals.push_back(glm::vec3(xPos, yPos, zPos));
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

	std::vector<float> data;
	for (int i = 0; i < m_Positions.size(); i++) {
		data.push_back(m_Positions[i].x);
		data.push_back(m_Positions[i].y);
		data.push_back(m_Positions[i].z);
		if (m_Normals.size() > 0) {
			data.push_back(m_Normals[i].x);
			data.push_back(m_Normals[i].y);
			data.push_back(m_Normals[i].z);
		}
		if (m_Uvs.size() > 0) {
			data.push_back(m_Uvs[i].x);
			data.push_back(m_Uvs[i].y);
		}
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
	float stride = (3 + 3 + 2) * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}


Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void Sphere::Render(Shader shader)
{
	shader.use();
	if (GetUseTexture()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	shader.setInt("useTexture", m_UseTexture);
	shader.setMatrix4("model", GetWorldTransform());
	shader.setVec3("cubeColor", m_Color);
	shader.setInt("highLight", GetHighLight());
	shader.setVec3("highLightColor", GetHighLightColor());
	shader.setInt("nodeID", GetID());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::SetTexture(const std::string& path)
{
	SetUseTexture(true);
	m_TextureID = Texture::loadTexture(path);
}

void Sphere::SetColor(const glm::vec3& color)
{
	if (m_Color != color) {
		m_Color = color;
	}
}

glm::vec3& Sphere::GetColor()
{
	return m_Color;
}

void Sphere::SetWorldTransform(const glm::mat4& mat)
{
	if (m_WorldTransform != mat) {
		m_WorldTransform = mat;
	}
}

glm::mat4& Sphere::GetWorldTransform()
{
	return m_WorldTransform;
}

void Sphere::SetUseTexture(bool value)
{
	m_UseTexture = value;
}

bool Sphere::GetUseTexture()
{
	return m_UseTexture;
}

void Sphere::SetHighLight(bool value)
{
	m_HighLight = value;
}

bool Sphere::GetHighLight()
{
	return m_HighLight;
}

void Sphere::SetHighLightColor(const glm::vec3& color)
{
	if (m_HighLightColor != color) {
		m_HighLightColor = color;
	}
}

glm::vec3& Sphere::GetHighLightColor()
{
	return m_HighLightColor;
}

void Sphere::SetID(unsigned long id)
{
	m_ID = id;
}

unsigned long Sphere::GetID()
{
	return m_ID;
}
