#pragma once
#include "BaseComponent.h"
#include "SpherePrimitive.h"

class SphereComponent : public BaseComponent
{
public:
	SphereComponent();
	SphereComponent(const std::string& diffuse, const std::string& normal, const std::string& specular);
	~SphereComponent();

	virtual void Update(double curFrame, double deltaFrame);

	virtual stringT GetTypeName();

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	SpherePrimitive m_Sphere;
};

