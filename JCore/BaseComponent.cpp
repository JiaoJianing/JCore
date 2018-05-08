#include "stdafx.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent()
	: m_Owner(0)
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
