#include "stdafx.h"
#include "SnowParticleSystem.h"
#include "ResourceManager.h"

SnowParticleSystem::SnowParticleSystem()
{
	m_ParticleTexture = *ResourceManager::getInstance()->LoadTexture("snow", "asset/resources/White snow.png");
}


SnowParticleSystem::~SnowParticleSystem()
{
}

void SnowParticleSystem::Initialize(int particleCount)
{
	__super::Initialize(particleCount);

	m_Particles = new Particle[m_ParticleCount];
	m_InstanceData = new InstanceData[m_ParticleCount];
	memset(m_Particles, 0, sizeof(Particle) * m_ParticleCount);
	memset(m_InstanceData, 0, sizeof(InstanceData) * m_ParticleCount);

	for (int i = 0; i < m_ParticleCount; i++) {
		initParticle(m_Particles[i]);
	}
}

void SnowParticleSystem::Update()
{
	for (int i = 0; i < m_ParticleCount; i++) {
		float x = m_Particles[i].Position.x;
		float y = m_Particles[i].Position.y;
		float z = m_Particles[i].Position.z;
		float vx = m_Particles[i].Velocity.x;
		float vy = m_Particles[i].Velocity.y;
		float vz = m_Particles[i].Velocity.z;
		float ax = m_Particles[i].aVelocity.x;
		float ay = m_Particles[i].aVelocity.y;
		float az = m_Particles[i].aVelocity.z;

		/** 更新位置 */
		x += (vx * 5);
		y -= vy;

		/** 更新速度 */
		vy += ay;

		/** 更新生存时间 */
		m_Particles[i].Life -= m_Particles[i].Decrease;

		//if (x > 3)
			//x = -2;

		/** 如果粒子消失或生命结束 */
		if (y <= -1 || m_Particles[i].Life <= 0)
		{
			initParticle(m_Particles[i]);
		}
		else {
			m_Particles[i].Position = glm::vec3(x, y, z);
			m_Particles[i].Velocity = glm::vec3(vx, vy, vz);
			m_Particles[i].aVelocity = glm::vec3(ax, ay, az);
		}

		m_InstanceData[i].Position = m_Particles[i].Position;
		m_InstanceData[i].Color = m_Particles[i].Color;
		m_InstanceData[i].Size = m_Particles[i].Size;
	}
}

void SnowParticleSystem::initParticle(Particle& particle)
{
	float r = 0.1f * (m_Random() % 10);
	float g = 0.1f * (m_Random() % 10);
	float b = 0.1f * (m_Random() % 10);
	particle.Color = glm::vec3(r, g, b);

	float x = -10.0f + 0.0001f * (m_Random() % 200000);
	float y = 3 + 0.0001f *(m_Random() % 200000);
	float z = 0.0f;
	if ((int)x % 2 == 0)
		z = 0.0001f * (m_Random() % 100000);
	else
		z = -(0.0001f * (m_Random() % 100000));

	particle.Position = glm::vec3(x, y, z);

	particle.Velocity = glm::vec3(0.00001 * (m_Random() % 100), 0.0000002 * (m_Random() % 28000), 0);

	particle.aVelocity = glm::vec3(0, 0.000005f, 0);

	particle.Life = 100;

	particle.Decrease = 0.005 * (m_Random() % 50);

	particle.Size = 0.01f * (m_Random() % 15);
}
