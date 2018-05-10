#include "stdafx.h"
#include "BaseLight.h"

BaseLight::BaseLight()
	: m_LightColor(1.0f)
	, m_LightPos(0.0f)
	, m_AmbientIntensity(0.01f)
	, m_DiffuseIntensity(1.0f)
{
}

BaseLight::~BaseLight()
{
}

glm::vec3& BaseLight::GetLightColor()
{
	return m_LightColor;
}

void BaseLight::SetLightColor(const glm::vec3& value)
{
	if (m_LightColor != value) {
		m_LightColor = value;
	}
}

glm::vec3& BaseLight::GetLightPos()
{
	return m_LightPos;
}

void BaseLight::SetLightPos(const glm::vec3& value)
{
	if (m_LightPos != value) {
		m_LightPos = value;
	}
}

float BaseLight::GetAmbientIntensity()
{
	return m_AmbientIntensity;
}

void BaseLight::SetAmbientIntensity(float value)
{
	m_AmbientIntensity = value;
}

float BaseLight::GetDiffuseIntensity()
{
	return m_DiffuseIntensity;
}

void BaseLight::SetDiffuseIntensity(float value)
{
	m_DiffuseIntensity = value;
}
