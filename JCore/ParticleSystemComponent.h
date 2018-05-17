#pragma once
#include "BaseComponent.h"

class ParticleSystemComponent : public BaseComponent
{
public:
	ParticleSystemComponent();
	~ParticleSystemComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual void Initialize(int particleCount);
};

