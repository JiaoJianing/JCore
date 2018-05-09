#pragma once
#include <glm.hpp>
#include "Shader.h"
#include <vector>
#include "Mesh.h"

class Cube {
public:
	Cube();
	~Cube();

	void Render(Shader shader);

	void SetDiffuseTexture(const std::string& path);
	void SetNormalTexture(const std::string& path);
	void SetSpecularTexture(const std::string& path);

	void SetColor(const glm::vec3& color);
	glm::vec3& GetColor();

	void SetWorldTransform(const glm::mat4& mat);
	glm::mat4& GetWorldTransform();

	void SetHighLight(bool value);
	bool GetHighLight();

	void SetHighLightColor(const glm::vec3& color);
	glm::vec3& GetHighLightColor();

	void SetID(unsigned long id);
	unsigned long GetID();

private:
	glm::vec3 m_Color;
	unsigned int m_VBO, m_VAO, m_EBO;
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	Texture m_DiffuseTexture;
	Texture m_NormalTexture;
	Texture m_SpecularTexture;

	glm::mat4 m_WorldTransform;
	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	unsigned long m_ID;
};