#include "stdafx.h"
#include "BaseLightComponent.h"

BaseLightComponent::BaseLightComponent()
{
}


BaseLightComponent::~BaseLightComponent()
{
}

stringT BaseLightComponent::GetTypeName()
{
	return _T("BaseLightComponent");
}

void BaseLightComponent::Update(double curFrame, double deltaFrame)
{
}

void BaseLightComponent::OnAddToWorld(World* world)
{
}

void BaseLightComponent::OnRemoveFromWorld(World* world)
{
}

glm::vec3& BaseLightComponent::GetLightColor()
{
	return GetLight()->GetLightColor();
}

void BaseLightComponent::SetLightColor(const glm::vec3& value)
{
	GetLight()->SetLightColor(value);
}
