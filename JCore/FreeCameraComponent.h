#pragma once
#include "CameraComponent.h"

class FreeCameraComponent : public CameraComponent
{
public:
	FreeCameraComponent(int width, int height);
	~FreeCameraComponent();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnKeyboard(int key);

	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset);

	virtual stringT GetTypeName();

private:
	float m_Yaw;
	float m_Pitch;
	float m_Roll;

	float m_HeightOffset;
};

