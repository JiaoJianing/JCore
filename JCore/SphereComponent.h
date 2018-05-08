#pragma once
#include "BaseComponent.h"
#include "Sphere.h"

class SphereComponent : public BaseComponent
{
public:
	SphereComponent();
	SphereComponent(const std::string& texturePath);
	~SphereComponent();

	virtual void Update(double curFrame, double deltaFrame);

	virtual stringT GetTypeName();

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	Sphere m_Sphere;
};

