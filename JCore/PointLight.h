#pragma once
#include "BaseLight.h"

//���Դ Ŀǰ�������֧��10��
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
	//˥��ϵ��
	float m_Constant;
	float m_Linear;
	float m_Exp;
};

