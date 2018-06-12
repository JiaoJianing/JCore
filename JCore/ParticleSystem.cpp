#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	: m_ParticleCount(0)
	, m_Particles(0)
	, m_InstanceData(0)
	, m_IsActive(true)
{
}


ParticleSystem::~ParticleSystem()
{
	if (m_Particles != 0) {
		delete[] m_Particles;
		m_Particles = 0;
	}

	if (m_InstanceData != 0) {
		delete[] m_InstanceData;
		m_InstanceData = 0;
	}

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_InstanceVBO);
}

void ParticleSystem::Initialize(int particleCount)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_InstanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, particleCount * sizeof(InstanceData), 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(2 * sizeof(glm::vec3)));

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);

	m_ParticleCount = particleCount;
}

void ParticleSystem::Update()
{

}

void ParticleSystem::Render(Shader shader)
{
	if (!GetIsActive()) {
		return;
	}

	if (m_ParticleCount > 0) {
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ParticleTexture.GetID());
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_ParticleCount * sizeof(InstanceData), &m_InstanceData[0]);
		glDrawArraysInstanced(GL_POINTS, 0, 1, m_ParticleCount);
		glBindVertexArray(0);
	}
}

bool ParticleSystem::GetIsActive()
{
	return m_IsActive;
}

void ParticleSystem::SetIsActive(bool value)
{
	m_IsActive = value;
}
