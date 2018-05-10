#include "stdafx.h"
#include "SpotLightComponent.h"
#include "World.h"

SpotLightComponent::SpotLightComponent()
{
}


SpotLightComponent::~SpotLightComponent()
{
}

stringT SpotLightComponent::GetTypeName()
{
	return _T("SpotLightComponent");
}

void SpotLightComponent::Update(double curFrame, double deltaFrame)
{
	m_SpotLight.SetLightPos(GetOwner()->GetWorldTransform()[3]);
}

void SpotLightComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetLights().push_back(&m_SpotLight);
}

void SpotLightComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetLights().begin(), world->GetScene()->GetLights().end(), &m_SpotLight);
	if (it != world->GetScene()->GetLights().end()) {
		world->GetScene()->GetLights().erase(it);
	}
}

BaseLight* SpotLightComponent::GetLight()
{
	return &m_SpotLight;
}

float SpotLightComponent::GetCutOff()
{
	return m_SpotLight.GetCutOff();
}

void SpotLightComponent::SetCutOff(float value)
{
	m_SpotLight.SetCutOff(value);
}

glm::vec3& SpotLightComponent::GetDirection()
{
	return m_SpotLight.GetDirection();
}

void SpotLightComponent::SetDirection(const glm::vec3& value)
{
	m_SpotLight.SetDirection(value);
}
