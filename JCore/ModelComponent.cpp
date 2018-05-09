#include "stdafx.h"
#include "Node.h"
#include "ModelComponent.h"
#include "ResourceManager.h"
#include "World.h"

ModelComponent::ModelComponent(const std::string& path)
{
	m_Model.LoadModel(path.c_str());
}

ModelComponent::~ModelComponent()
{
}

stringT ModelComponent::GetTypeName()
{
	return _T("ModelComponent");
}

void ModelComponent::Update(double curFrame, double deltaFrame)
{
	m_Model.SetWorldTransform(GetOwner()->GetWorldTransform());
	m_Model.SetHighLight(GetOwner()->GetHighLight());
	m_Model.SetHighLightColor(GetOwner()->GetHighLightColor());
	m_Model.SetID(GetOwner()->GetID());
	m_Model.SetColor(GetOwner()->GetColor());
}

void ModelComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetModels().push_back(&m_Model);
}

void ModelComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetModels().begin(), world->GetScene()->GetModels().end(), &m_Model);
	if (it != world->GetScene()->GetModels().end()) {
		world->GetScene()->GetModels().erase(it);
	}
}
