#include "stdafx.h"
#include "CubeComponent.h"
#include "ResourceManager.h"
#include "Node.h"
#include "Texture.h"
#include "World.h"

CubeComponent::CubeComponent()
{
	m_Cube.SetDiffuseTexture("asset/resources/default_diffuse.jpg");
	m_Cube.SetNormalTexture("asset/resources/default_normal.jpg");
	m_Cube.SetSpecularTexture("asset/resources/default_specular.jpg");
}


CubeComponent::CubeComponent(const std::string& diffuse, const std::string& normal, const std::string& specular)
	: CubeComponent()
{
	m_Cube.SetDiffuseTexture(diffuse);
	m_Cube.SetNormalTexture(normal);
	m_Cube.SetSpecularTexture(specular);
}

CubeComponent::~CubeComponent()
{
}

void CubeComponent::Update(double curFrame, double deltaFrame)
{
	m_Cube.SetWorldTransform(GetOwner()->GetWorldTransform());
	m_Cube.SetHighLight(GetOwner()->GetHighLight());
	m_Cube.SetHighLightColor(GetOwner()->GetHighLightColor());
	m_Cube.SetColor(GetOwner()->GetColor());
	m_Cube.SetID(GetOwner()->GetID());
}

stringT CubeComponent::GetTypeName()
{
	return _T("CubeComponent");
}

void CubeComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetCustomPrimitives().push_back(&m_Cube);
}

void CubeComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetCustomPrimitives().begin(), world->GetScene()->GetCustomPrimitives().end(), &m_Cube);
	if (it != world->GetScene()->GetCustomPrimitives().end()) {
		world->GetScene()->GetCustomPrimitives().erase(it);
	}
}
