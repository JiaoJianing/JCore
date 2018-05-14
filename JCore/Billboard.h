#pragma once
#include "Shader.h"
#include "Texture.h"

class Billboard
{
public:
	Billboard();
	~Billboard();

	void Render(Shader shader);

	void SetWorldPos(const glm::vec3& pos);
	glm::vec3& GetWorldPos();

	void SetTexture(const Texture& texture);
	Texture& GetTexture();

private:
	unsigned int m_VAO, m_VBO;
	glm::vec3 m_WorldPos;
	Texture m_Texture;
};