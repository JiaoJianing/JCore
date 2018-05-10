#pragma once
class BaseLight
{
public:
	BaseLight();
	virtual ~BaseLight();

	glm::vec3& GetLightColor();
	void SetLightColor(const glm::vec3& value);

	glm::vec3& GetLightPos();
	void SetLightPos(const glm::vec3& value);

	float GetAmbientIntensity();
	void SetAmbientIntensity(float value);

	float GetDiffuseIntensity();
	void SetDiffuseIntensity(float value);

protected:
	glm::vec3 m_LightColor;
	glm::vec3 m_LightPos;
	float m_AmbientIntensity;
	float m_DiffuseIntensity;
};

