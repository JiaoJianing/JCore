#include "SpriteRenderer.h"
#include <gtc/matrix_transform.hpp>

float spriteVertices[] = {
	//Œª÷√ //Œ∆¿Ì
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
};

SpriteRenderer::SpriteRenderer(Shader shader)
{
	m_Shader = shader;
	initRenderData();
}


SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void SpriteRenderer::DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size /*= glm::vec2(10, 10)*/, float rotate /*= 0.0f*/, glm::vec3 color /*= glm::vec3(1.0f)*/)
{
	m_Shader.use();
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader.setMatrix4("model", model);
	m_Shader.setVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetID());
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
