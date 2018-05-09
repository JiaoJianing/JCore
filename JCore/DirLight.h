#pragma once
#include "BaseLight.h"
#include <glm.hpp>

class DirLight : public BaseLight
{
public:
	DirLight();
	~DirLight();

	float GetAmbientIntensity();
	void SetAmbientIntensity(float value);

	float GetDiffuseIntensity();
	void SetDiffuseIntensity(float value);

private:
	float m_AmbientIntensity;
	float m_DiffuseIntensity;
};

