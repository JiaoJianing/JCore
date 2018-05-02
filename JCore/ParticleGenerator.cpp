#include "ParticleGenerator.h"
#include <GLFW/glfw3.h>

float particleVertices[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
	: m_Shader(shader), m_Texture(texture), m_Amount(amount), m_LastUseParticle(0)
{
	this->init();
}

ParticleGenerator::~ParticleGenerator()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset /*= glm::vec2(0.0f)*/)
{
	//�����µ�����
	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->m_Particles[unusedParticle], object, offset);
	}

	//�����������ӵ���Ϣ
	for (unsigned int i = 0; i < this->m_Amount; i++) {
		Particle& p = this->m_Particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f) {
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	//�����ܼ��������glowЧ��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->m_Shader.use();
	for (Particle &particle : this->m_Particles) {
		if (particle.Life > 0.0f) {
			this->m_Shader.setVec2("offset", particle.Position);
			this->m_Shader.setVec4("color", particle.Color);
			glBindTexture(GL_TEXTURE_2D, m_Texture.GetID());
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//�ָ�Ĭ�ϻ�Ϸ�ʽ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//��Ĭ�ϵ������������������
	for (unsigned int i = 0; i < this->m_Amount; i++) {
		this->m_Particles.push_back(Particle());
	}
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
	//����m_LastUseParticle֮�󴴽�������
	for (unsigned int i = m_LastUseParticle; i < this->m_Amount; i++) {
		if (this->m_Particles[i].Life <= 0.0f) {
			m_LastUseParticle = i;
			return i;
		}
	}

	//��û���ҵ������0���Ա���
	for (unsigned int i = 0; i < m_LastUseParticle; i++) {
		if (this->m_Particles[i].Life <= 0.0f) {
			m_LastUseParticle = i;
			return i;
		}
	}

	m_LastUseParticle = 0;
	return m_LastUseParticle;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset /*= glm::vec2(0.0f)*/)
{
	float random = ((rand() % 100) - 50) / 10.0f; //[-5, 5]
	float rColor = 0.5 + ((rand() % 100) / 100.0f); //[0.5, 1.5]

	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
