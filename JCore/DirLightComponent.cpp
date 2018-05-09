#include "stdafx.h"
#include "DirLightComponent.h"
#include "World.h"

DirLightComponent::DirLightComponent()
{
}


DirLightComponent::~DirLightComponent()
{
}

stringT DirLightComponent::GetTypeName()
{
	return _T("DirLightComponent");
}

void DirLightComponent::Update(double curFrame, double deltaFrame)
{
	m_DirLight.SetLightColor(GetLightColor());
	m_DirLight.SetLightPos(GetOwner()->GetWorldTransform()[3]);
	m_DirLight.SetAmbientIntensity(GetAmbientIntensity());
	m_DirLight.SetDiffuseIntensity(GetDiffuseIntensity());
}

void DirLightComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetLights().push_back(&m_DirLight);
}

void DirLightComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetLights().begin(), world->GetScene()->GetLights().end(), &m_DirLight);
	if (it != world->GetScene()->GetLights().end()) {
		world->GetScene()->GetLights().erase(it);
	}
}

BaseLight* DirLightComponent::GetLight()
{
	return &m_DirLight;
}

float DirLightComponent::GetAmbientIntensity()
{
	return m_DirLight.GetAmbientIntensity();
}

void DirLightComponent::SetAmbientIntensity(float value)
{
	m_DirLight.SetAmbientIntensity(value);
}

float DirLightComponent::GetDiffuseIntensity()
{
	return m_DirLight.GetDiffuseIntensity();
}

void DirLightComponent::SetDiffuseIntensity(float value)
{
	m_DirLight.SetDiffuseIntensity(value);
}
