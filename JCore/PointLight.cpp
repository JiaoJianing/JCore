#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight()
	: m_Constant(1.0f)
	, m_Linear(0.0f)
	, m_Exp(0.0f)
{
}


PointLight::~PointLight()
{
}

float PointLight::GetConstant()
{
	return m_Constant;
}

void PointLight::SetConstant(float value)
{
	m_Constant = value;
}

float PointLight::GetLinear()
{
	return m_Linear;
}

void PointLight::SetLinear(float value)
{
	m_Linear = value;
}

float PointLight::GetExp()
{
	return m_Exp;
}

void PointLight::SetExp(float value)
{
	m_Exp = value;
}
