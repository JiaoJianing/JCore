#include "stdafx.h"
#include "SphereComponent.h"
#include "World.h"

SphereComponent::SphereComponent()
{
	m_Sphere.SetDiffuseTexture("asset/resources/default_diffuse.jpg");
	m_Sphere.SetNormalTexture("asset/resources/default_normal.jpg");
	m_Sphere.SetSpecularTexture("asset/resources/default_specular.jpg");
}


SphereComponent::SphereComponent(const std::string& diffuse, const std::string& normal, const std::string& specular)
{
	m_Sphere.SetDiffuseTexture(diffuse);
	m_Sphere.SetNormalTexture(normal);
	m_Sphere.SetSpecularTexture(specular);
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
