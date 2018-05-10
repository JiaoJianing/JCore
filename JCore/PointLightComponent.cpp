#include "stdafx.h"
#include "PointLightComponent.h"
#include "World.h"

PointLightComponent::PointLightComponent()
{
}


PointLightComponent::~PointLightComponent()
{
}

stringT PointLightComponent::GetTypeName()
{
	return _T("PointLightComponent");
}

void PointLightComponent::Update(double curFrame, double deltaFrame)
{
	m_PointLight.SetLightColor(GetLightColor());
	m_PointLight.SetLightPos(GetOwner()->GetWorldTransform()[3]);
	m_PointLight.SetAmbientIntensity(GetAmbientIntensity());
	m_PointLight.SetDiffuseIntensity(GetDiffuseIntensity());
	m_PointLight.SetConstant(GetConstant());
	m_PointLight.SetLinear(GetLinear());
	m_PointLight.SetExp(GetExp());
}

void PointLightComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetLights().push_back(&m_PointLight);
}

void PointLightComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetLights().begin(), world->GetScene()->GetLights().end(), &m_PointLight);
	if (it != world->GetScene()->GetLights().end()) {
		world->GetScene()->GetLights().erase(it);
	}
}

BaseLight* PointLightComponent::GetLight()
{
	return &m_PointLight;
}

float PointLightComponent::GetConstant()
{
	return m_PointLight.GetConstant();
}

void PointLightComponent::SetConstant(float value)
{
	m_PointLight.SetConstant(value);
}

float PointLightComponent::GetLinear()
{
	return m_PointLight.GetLinear();
}

void PointLightComponent::SetLinear(float value)
{
	m_PointLight.SetLinear(value);
}

float PointLightComponent::GetExp()
{
	return m_PointLight.GetExp();
}

void PointLightComponent::SetExp(float value)
{
	m_PointLight.SetExp(value);
}
