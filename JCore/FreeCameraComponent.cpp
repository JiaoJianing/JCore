#include "stdafx.h"
#include "FreeCameraComponent.h"
#include "World.h"

FreeCameraComponent::FreeCameraComponent(int width, int height)
	: CameraComponent(width, height)
	, m_Yaw(-90.0f)
	, m_Pitch(0.0f)
	, m_Roll(0.0f)
	, m_HeightOffset(7.0f)
	, m_FlyMode(false)
{
	m_KeySensitivity = 10.0f;
}


FreeCameraComponent::~FreeCameraComponent()
{
}

void FreeCameraComponent::Update(double curFrame, double deltaFrame)
{
	__super::Update(curFrame, deltaFrame);

	if (GetWorld() != 0) {
		glm::vec3 position = m_Camera.GetPosition();
		if (!m_FlyMode) {
			float height = GetWorld()->GetHeightAt(position);
			if (height > 0) {
				position.y = height + m_HeightOffset;
				m_Camera.SetPosition(position);
			}
		}
	}
}

void FreeCameraComponent::OnKeyboard(int key)
{
	float cameraSpeed = m_KeySensitivity * m_DeltaFrame;

	switch (key)
	{
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
		m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
		break;
	case GLFW_KEY_DOWN:
	case GLFW_KEY_S:
		m_Camera.SetPosition(m_Camera.GetPosition() - cameraSpeed * m_Target);
		break;
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		m_Camera.SetPosition(m_Camera.GetPosition() - glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed);
		break;
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed);
		break;
	default:
		break;
	}
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

	m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
}

stringT FreeCameraComponent::GetTypeName()
{
	return _T("FreeCameraComponent");
}

void FreeCameraComponent::ToFlyMode(bool value)
{
	m_FlyMode = value;
}

bool& FreeCameraComponent::GetFlyMode()
{
	return m_FlyMode;
}
