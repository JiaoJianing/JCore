#pragma once
#include <glm.hpp>
#include "Shader.h"

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
	unsigned int m_VBO, m_VAO;
	unsigned int m_DiffuseTexture;
	unsigned int m_NormalTexture;
	unsigned int m_SpecularTexture;

	glm::mat4 m_WorldTransform;
	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	unsigned long m_ID;
};