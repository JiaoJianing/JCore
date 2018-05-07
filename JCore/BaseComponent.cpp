#include "stdafx.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent()
	: m_Owner(0)
	, m_HighLight(false)
	, m_HighLightColor(1.0f, 1.0f, 0.0f)
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

bool BaseComponent::GetHighLight()
{
	return m_HighLight;
}

void BaseComponent::SetHighLight(bool value)
{
	if (value != m_HighLight) {
		m_HighLight = value;
	}
}

const glm::vec3& BaseComponent::GetHighLightColor() const
{
	return m_HighLightColor;
}

void BaseComponent::SetHighLightColor(const glm::vec3& color)
{
	if (m_HighLightColor != color) {
		m_HighLightColor = color;
	}
}
