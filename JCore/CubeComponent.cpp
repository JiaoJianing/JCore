#include "stdafx.h"
#include "CubeComponent.h"
#include "ResourceManager.h"
#include "Node.h"
#include "Texture.h"
#include "World.h"

CubeComponent::CubeComponent()
{
}


CubeComponent::CubeComponent(const std::string& texturePath)
	: CubeComponent()
{
	m_Cube.SetTexture(texturePath);
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
	world->GetScene()->GetCubes().push_back(&m_Cube);
}

void CubeComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetCubes().begin(), world->GetScene()->GetCubes().end(), &m_Cube);
	if (it != world->GetScene()->GetCubes().end()) {
		world->GetScene()->GetCubes().erase(it);
	}
}
