#pragma once
#include "ParticleSystemComponent.h"
#include "SnowParticleSystem.h"

class SnowParticleSystemComponent : public ParticleSystemComponent
{
public:
	SnowParticleSystemComponent();
	~SnowParticleSystemComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual void Initialize(int particleCount);

private:
	SnowParticleSystem m_SnowParticleSys;
};

