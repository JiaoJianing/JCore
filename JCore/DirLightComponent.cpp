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
	m_DirLight.SetLightPos(GetOwner()->GetWorldTransform()[3]);
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