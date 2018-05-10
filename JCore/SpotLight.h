#pragma once
#include "PointLight.h"

//聚光灯，目前支持最多10个
class SpotLight : public PointLight
{
public:
	SpotLight();
	~SpotLight();

	float GetCutOff();
	void SetCutOff(float value);

	glm::vec3& GetDirection();
	void SetDirection(const glm::vec3& value);

private:
	glm::vec3 m_Direction;
	float m_CutOff;
};

