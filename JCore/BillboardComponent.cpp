#include "stdafx.h"
#include "BillboardComponent.h"
#include "World.h"
#include "ResourceManager.h"

BillboardComponent::BillboardComponent(const std::string& texturePath)
{
	m_Billboard.SetTexture(*ResourceManager::getInstance()->LoadTexture(texturePath, texturePath));
}


BillboardComponent::~BillboardComponent()
{
}

stringT BillboardComponent::GetTypeName()
{
	return _T("BillboardComponent");
}

void BillboardComponent::Update(double curFrame, double deltaFrame)
{
	if (GetOwner()) {
		m_Billboard.SetWorldPos(GetOwner()->GetWorldTransform()[3]);
	}
}

void BillboardComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetBillboards().push_back(&m_Billboard);
}

void BillboardComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetBillboards().begin(), world->GetScene()->GetBillboards().end(), &m_Billboard);
	if (it != world->GetScene()->GetBillboards().end()) {
		world->GetScene()->GetBillboards().erase(it);
	}
}
