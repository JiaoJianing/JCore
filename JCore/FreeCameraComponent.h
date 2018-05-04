#pragma once
#include "CameraComponent.h"

class FreeCameraComponent : public CameraComponent
{
public:
	FreeCameraComponent(int width, int height);
	~FreeCameraComponent();

	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset);

	virtual std::string GetTypeName();

private:
	float m_Yaw;
	float m_Pitch;
	float m_Roll;
};

