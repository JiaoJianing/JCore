#include "stdafx.h"
#include "Billboard.h"

float billboardVertices[] = {
	0, 0, 0
};

Billboard::Billboard()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(billboardVertices), billboardVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Billboard::~Billboard()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Billboard::Render(Shader shader)
{
	shader.use();
	glm::mat4 model;
	shader.setMatrix4("model", glm::translate(model, m_WorldPos));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetID());

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void Billboard::SetWorldPos(const glm::vec3& pos)
{
	if (m_WorldPos != pos) {
		m_WorldPos = pos;
	}
}

glm::vec3& Billboard::GetWorldPos()
{
	return m_WorldPos;
}

void Billboard::SetTexture(const Texture& texture)
{
	m_Texture = texture;
}

Texture& Billboard::GetTexture()
{
	return m_Texture;
}
