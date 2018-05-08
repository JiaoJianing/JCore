#pragma once
#include "Shader.h"
#include <vector>

class Sphere
{
public:
	Sphere();
	~Sphere();

	void Render(Shader shader);

	void SetTexture(const std::string& path);

	void SetColor(const glm::vec3& color);
	glm::vec3& GetColor();

	void SetWorldTransform(const glm::mat4& mat);
	glm::mat4& GetWorldTransform();

	void SetUseTexture(bool value);
	bool GetUseTexture();

	void SetHighLight(bool value);
	bool GetHighLight();

	void SetHighLightColor(const glm::vec3& color);
	glm::vec3& GetHighLightColor();

	void SetID(unsigned long id);
	unsigned long GetID();

private:
	unsigned int m_VAO, m_VBO, m_EBO;
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec2> m_Uvs;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned int> m_Indices;

	unsigned int m_TextureID;
	bool m_UseTexture;

	glm::mat4 m_WorldTransform;
	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	unsigned long m_ID;
	glm::vec3 m_Color;
};

