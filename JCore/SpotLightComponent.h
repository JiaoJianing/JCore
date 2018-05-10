#pragma once
#include "PointLightComponent.h"
#include "SpotLight.h"

class SpotLightComponent : public PointLightComponent
{
public:
	SpotLightComponent();
	~SpotLightComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual BaseLight* GetLight();

public:
	float GetCutOff();
	void SetCutOff(float value);

	glm::vec3& GetDirection();
	void SetDirection(const glm::vec3& value);

private:
	SpotLight m_SpotLight;
};

