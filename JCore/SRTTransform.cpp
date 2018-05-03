#include "stdafx.h"
#include "SRTTransform.h"

SRTTransform::SRTTransform()
	: m_Scale(1.0f)
	, m_Rotate(0.0f)
	, m_Translation(0.0f)
	, m_IsDirty(false)
{
}


SRTTransform::~SRTTransform()
{
}

const glm::vec3& SRTTransform::GetScale() const
{
	return m_Scale;
}

void SRTTransform::SetScale(const glm::vec3& scale)
{
	if (m_Scale != scale) {
		m_Scale = scale;
		m_IsDirty = true;
	}
}

const glm::vec3& SRTTransform::GetRotate() const
{
	return m_Rotate;
}

void SRTTransform::SetRotate(const glm::vec3& rotate)
{
	if (m_Rotate != rotate) {
		m_Rotate = rotate;
		m_IsDirty = true;
	}
}

const glm::vec3 SRTTransform::GetTranslation() const
{
	return m_Translation;
}

void SRTTransform::SetTranslation(const glm::vec3& translate)
{
	if (m_Translation != translate) {
		m_Translation = translate;
		m_IsDirty = true;
	}
}

const glm::mat4& SRTTransform::GetTransformMatrix()
{
	if (m_IsDirty) {

		m_TransformMatrix = glm::mat4();

		m_TransformMatrix = glm::translate(m_TransformMatrix, m_Translation);

		m_TransformMatrix = glm::rotate(m_TransformMatrix, glm::radians(m_Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_TransformMatrix = glm::rotate(m_TransformMatrix, glm::radians(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_TransformMatrix = glm::rotate(m_TransformMatrix, glm::radians(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);

		m_IsDirty = false;
	}

	return m_TransformMatrix;
}

bool SRTTransform::GetIsDirty()
{
	return m_IsDirty;
}
