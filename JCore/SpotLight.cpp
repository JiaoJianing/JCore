#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SpotLight()
	: m_CutOff(20.0f)
{
}


SpotLight::~SpotLight()
{
}

float SpotLight::GetCutOff()
{
	return m_CutOff;
}

void SpotLight::SetCutOff(float value)
{
	m_CutOff = value;
}

glm::vec3& SpotLight::GetDirection()
{
	return m_Direction;
}

void SpotLight::SetDirection(const glm::vec3& value)
{
	if (m_Direction != value) {
		m_Direction = value;
	}
}
