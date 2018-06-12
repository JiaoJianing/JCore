#pragma once
#include <glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include <random>

struct Particle {
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Velocity;
	glm::vec3 aVelocity;
	float Life;
	float Size;
	float Decrease;
};

struct InstanceData
{
	glm::vec3 Position;
	glm::vec3 Color;
	float Size;
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	virtual void Initialize(int particleCount);

	virtual void Update();

	void Render(Shader shader);

	bool GetIsActive();
	void SetIsActive(bool value);

protected:
	int m_ParticleCount;
	Particle* m_Particles;
	InstanceData* m_InstanceData;
	unsigned int m_VAO, m_InstanceVBO;
	std::random_device m_Random;
	Texture m_ParticleTexture;
	bool m_IsActive;
};

