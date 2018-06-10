#pragma once
#include "ParticleSystem.h"

class SnowParticleSystem : public ParticleSystem
{
public:
	SnowParticleSystem();
	~SnowParticleSystem();

	virtual void Initialize(int particleCount);

	virtual void Update();

private:
	void initParticle(Particle& particle);

private:
	float m_SnowRange;
};

