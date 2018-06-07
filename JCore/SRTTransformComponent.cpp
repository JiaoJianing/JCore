#include "stdafx.h"
#include "SRTTransformComponent.h"
#include "Node.h"
#include "World.h"

SRTTransformComponent::SRTTransformComponent()
	: m_TransformDirty(false)
	, m_World(0)
	, m_CheckTerrain(true)
	, m_HeightAboveTerrain(0)
{
}


SRTTransformComponent::~SRTTransformComponent()
{
}

glm::vec3 SRTTransformComponent::GetScale()
{
	return m_SRT.GetScale();
}

void SRTTransformComponent::SetScale(const glm::vec3& scale)
{
	m_SRT.SetScale(scale);
	m_TransformDirty = true;
}

glm::vec3 SRTTransformComponent::GetRotate()
{
	return m_SRT.GetRotate();
}

void SRTTransformComponent::SetRotate(const glm::vec3& rotate)
{
	m_SRT.SetRotate(rotate);
	m_TransformDirty = true;
}

glm::vec3 SRTTransformComponent::GetTranslation()
{
	return m_SRT.GetTranslation();
}

void SRTTransformComponent::SetTranslation(const glm::vec3& translation)
{
	if (m_CheckTerrain && m_World != 0) {
		float height = m_World->GetHeightAt(translation);
		m_SRT.SetTranslation(glm::vec3(translation.x, height + m_HeightAboveTerrain, translation.z));
	}
	else {
		m_SRT.SetTranslation(translation);
	}
	m_TransformDirty = true;
}

bool SRTTransformComponent::GetCheckTerrain()
{
	return m_CheckTerrain;
}

void SRTTransformComponent::SetCheckTerrain(bool value)
{
	m_CheckTerrain = value;
	if (m_CheckTerrain) {
		if (m_CheckTerrain && m_World != 0) {
			float height = m_World->GetHeightAt(m_SRT.GetTranslation());
			m_SRT.SetTranslation(glm::vec3(m_SRT.GetTranslation().x, height + m_HeightAboveTerrain, m_SRT.GetTranslation().z));
			m_TransformDirty = true;
		}
	}
}

float SRTTransformComponent::GetHeightAboveTerrain()
{
	return m_HeightAboveTerrain;
}

void SRTTransformComponent::SetHeightAboveTerrain(float value)
{
	m_HeightAboveTerrain = value;
}

stringT SRTTransformComponent::GetTypeName()
{
	return _T("SRTTransformComponent");
}

void SRTTransformComponent::Update(double curFrame, double deltaFrame)
{
	if (GetOwner()->GetParent() == 0) {
		if (m_TransformDirty) {
			GetOwner()->SetWorldTransform(m_SRT.GetTransformMatrix());
			m_TransformDirty = false;
		}
	}
	else {
		if (GetOwner()->GetParent()->GetTransformDirty() || m_TransformDirty) {
			GetOwner()->SetWorldTransform(GetOwner()->GetParent()->GetWorldTransform() * m_SRT.GetTransformMatrix());
			m_TransformDirty = false;
		}
	}
}

void SRTTransformComponent::OnAddToWorld(World* world)
{
	m_World = world;
}

void SRTTransformComponent::OnRemoveFromWorld(World* world)
{

}
