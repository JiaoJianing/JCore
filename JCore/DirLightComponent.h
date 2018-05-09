#pragma once
#include "BaseLightComponent.h"
#include "DirLight.h"

class DirLightComponent : public BaseLightComponent
{
public:
	DirLightComponent();
	~DirLightComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual BaseLight* GetLight();

public:
	float GetAmbientIntensity();
	void SetAmbientIntensity(float value);

	float GetDiffuseIntensity();
	void SetDiffuseIntensity(float value);

private:
	DirLight m_DirLight;
};

