#include "stdafx.h"
#include "DirLight.h"

DirLight::DirLight()
	: m_AmbientIntensity(1.0f)
	, m_DiffuseIntensity(1.0f)
	, m_LightColor(1.0f)
	, m_LightPos(0.0f)
{
}


DirLight::~DirLight()
{
}

glm::vec3& DirLight::GetLightColor()
{
	return m_LightColor;
}

void DirLight::SetLightColor(const glm::vec3& value)
{
	if (m_LightColor != value) {
		m_LightColor = value;
	}
}

glm::vec3& DirLight::GetLightPos()
{
	return m_LightPos;
}

void DirLight::SetLightPos(const glm::vec3& value)
{
	if (m_LightPos != value) {
		m_LightPos = value;
	}
}

float DirLight::GetAmbientIntensity()
{
	return m_AmbientIntensity;
}

void DirLight::SetAmbientIntensity(float value)
{
	m_AmbientIntensity = value;
}

float DirLight::GetDiffuseIntensity()
{
	return m_DiffuseIntensity;
}

void DirLight::SetDiffuseIntensity(float value)
{
	m_DiffuseIntensity = value;
}
