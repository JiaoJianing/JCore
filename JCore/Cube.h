#pragma once
#include "Shader.h"
#include "Texture.h"

//Α’·½Με
class Cube
{
public:
	Cube();
	~Cube();

	void Draw(const glm::mat4& model);

	void SetColor(const glm::vec3& color);

private:
	glm::vec3 m_color;
	Texture m_texture;
	unsigned int m_VBO, m_VAO;
	unsigned int m_textureID;
	Shader m_Shader;
};

