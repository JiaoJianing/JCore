#pragma once
#include <glm.hpp>
#include "Shader.h"
#include <vector>

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
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec2> m_Uvs;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec3> m_Tangents;
	std::vector<glm::vec3> m_BiTangents;
	std::vector<unsigned int> m_Indices;

	unsigned int m_DiffuseTexture;
	unsigned int m_NormalTexture;
	unsigned int m_SpecularTexture;

	glm::mat4 m_WorldTransform;
	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	unsigned long m_ID;
};