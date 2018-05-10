#pragma once
#include "BaseLight.h"

//点光源 目前场景最多支持10个
class PointLight : public BaseLight
{
public:
	PointLight();
	~PointLight();

	float GetConstant();
	void SetConstant(float value);

	float GetLinear();
	void SetLinear(float value);

	float GetExp();
	void SetExp(float value);

private:
	//衰减系数
	float m_Constant;
	float m_Linear;
	float m_Exp;
};

