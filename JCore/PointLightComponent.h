#pragma once
#include "BaseLightComponent.h"
#include "PointLight.h"

class PointLightComponent : public BaseLightComponent
{
public:
	PointLightComponent();
	~PointLightComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual BaseLight* GetLight();

public:
	float GetConstant();
	void SetConstant(float value);

	float GetLinear();
	void SetLinear(float value);

	float GetExp();
	void SetExp(float value);

private:
	PointLight m_PointLight;
};

