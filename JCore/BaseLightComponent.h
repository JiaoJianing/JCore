#pragma once
#include "BaseComponent.h"
#include "BaseLight.h"

class BaseLightComponent : public BaseComponent
{
public:
	BaseLightComponent();
	~BaseLightComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual BaseLight* GetLight() = 0;

public:
	glm::vec3& GetLightColor();
	void SetLightColor(const glm::vec3& value);

	float GetAmbientIntensity();
	void SetAmbientIntensity(float value);

	float GetDiffuseIntensity();
	void SetDiffuseIntensity(float value);
};

