#include "stdafx.h"
#include "FreeCameraComponent.h"

FreeCameraComponent::FreeCameraComponent(int width, int height)
	: CameraComponent(width, height)
	, m_Yaw(-90.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
{
}


FreeCameraComponent::~FreeCameraComponent()
{
}

void FreeCameraComponent::OnMouseMove(double x, double y)
{
	if (m_MousePress) {
		__super::OnMouseMove(x, y);

		float xOffset = x - m_MouseX;
		float yOffset = m_MouseY - y;
		m_MouseX = x;
		m_MouseY = y;

		xOffset *= m_MouseSensitivity * m_DeltaFrame;
		yOffset *= m_MouseSensitivity * m_DeltaFrame;

		m_Yaw += xOffset;
		m_Pitch += yOffset;
		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		m_Target = glm::normalize(front);
	}
}

void FreeCameraComponent::OnMouseScroll(double xOffset, double yOffset)
{
	float cameraSpeed = m_KeySensitivity * yOffset * 0.1f;

	m_Pos += cameraSpeed * m_Target;
}

stringT FreeCameraComponent::GetTypeName()
{
	return _T("FreeCameraComponent");
}
