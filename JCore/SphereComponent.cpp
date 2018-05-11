#include "stdafx.h"
#include "SphereComponent.h"
#include "ResourceManager.h"
#include "Node.h"
#include "Texture.h"
#include "World.h"

SphereComponent::SphereComponent()
{
	m_Sphere.SetDiffuseTexture("asset/resources/default_diffuse.jpg");
	m_Sphere.SetNormalTexture("asset/resources/default_normal.jpg");
	m_Sphere.SetSpecularTexture("asset/resources/default_specular.jpg");
}


SphereComponent::SphereComponent(const std::string& diffuse, const std::string& normal, const std::string& specular)
	: SphereComponent()
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
	world->GetScene()->GetCustomPrimitives().push_back(&m_Sphere);
}

void SphereComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetCustomPrimitives().begin(), world->GetScene()->GetCustomPrimitives().end(), &m_Sphere);
	if (it != world->GetScene()->GetCustomPrimitives().end()) {
		world->GetScene()->GetCustomPrimitives().erase(it);
	}
}
