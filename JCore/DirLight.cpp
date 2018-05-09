#include "stdafx.h"
#include "DirLight.h"

DirLight::DirLight()
	: m_AmbientIntensity(1.0f)
	, m_DiffuseIntensity(1.0f)
{
}


DirLight::~DirLight()
{
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
