#pragma once
#include <glm.hpp>

class SRTTransform
{
public:
	SRTTransform();
	~SRTTransform();

	const glm::vec3& GetScale() const;
	void SetScale(const glm::vec3& scale);

	const glm::vec3& GetRotate()const;
	void SetRotate(const glm::vec3& rotate);

	const glm::vec3 GetTranslation()const;
	void SetTranslation(const glm::vec3& translate);

	const glm::mat4& GetTransformMatrix();

	bool GetIsDirty();

private:
	glm::vec3 m_Scale;
	glm::vec3 m_Rotate;
	glm::vec3 m_Translation;
	glm::mat4 m_TransformMatrix;

	bool m_IsDirty;
};

