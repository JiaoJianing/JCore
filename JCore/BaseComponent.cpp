#include "stdafx.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent()
	: m_Owner(0)
	, m_IsActive(true)
{
}


BaseComponent::~BaseComponent()
{
}

Node * BaseComponent::GetOwner()
{
	return m_Owner;
}

void BaseComponent::SetOwner(Node* value)
{
	m_Owner = value;
}

bool& BaseComponent::GetIsActive()
{
	return m_IsActive;
}

void BaseComponent::SetIsActive(bool value)
{
	m_IsActive = value;
}
