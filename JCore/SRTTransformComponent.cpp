#include "stdafx.h"
#include "SRTTransformComponent.h"
#include "Node.h"

SRTTransformComponent::SRTTransformComponent()
	: m_TransformDirty(false)
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
	m_SRT.SetTranslation(translation);
	m_TransformDirty = true;
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

void SRTTransformComponent::Render()
{

}
