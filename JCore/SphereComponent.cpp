#include "stdafx.h"
#include "SphereComponent.h"
#include "World.h"

SphereComponent::SphereComponent()
{
}


SphereComponent::SphereComponent(const std::string& texturePath)
{
	m_Sphere.SetTexture(texturePath);
}

SphereComponent::~SphereComponent()
{
}

void SphereComponent::Update(double curFrame, double deltaFrame)
{
	m_Sphere.SetWorldTransform(GetOwner()->GetWorldTransform());
	m_Sphere.SetHighLight(GetOwner()->GetHighLight());
	m_Sphere.SetHighLightColor(GetOwner()->GetHighLightColor());
	m_Sphere.SetColor(GetOwner()->GetColor());
	m_Sphere.SetID(GetOwner()->GetID());
}

stringT SphereComponent::GetTypeName()
{
	return _T("SphereComponent");
}

void SphereComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetSpheres().push_back(&m_Sphere);
}

void SphereComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetSpheres().begin(), world->GetScene()->GetSpheres().end(), &m_Sphere);
	if (it != world->GetScene()->GetSpheres().end()) {
		world->GetScene()->GetSpheres().erase(it);
	}
}
