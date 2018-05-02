#pragma once
#include "Shader.h"
#include "Texture.h"

//Α’·½Με
class Cube
{
public:
	Cube();
	~Cube();

	void Draw(Shader shader);

	void SetPos(const glm::vec3& pos);

	void SetScale(const glm::vec3& scale);

	void SetColor(const glm::vec3& color);

private:
	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::vec3 m_color;
	Texture m_texture;
	unsigned int m_VBO, m_VAO;
	unsigned int m_textureID;
};

