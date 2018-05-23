#include "stdafx.h"
#include "FollowCameraComponent.h"

FollowCameraComponent::FollowCameraComponent(int width, int height)
	: CameraComponent(width, height)
	, m_FollowNode(0)
	, m_ViewDistance(5)
	, m_MinViewDistance(1.0f)
	, m_Pitch(0)
{
}


FollowCameraComponent::~FollowCameraComponent()
{
}

stringT FollowCameraComponent::GetTypeName()
{
	return _T("FollowCameraComponent");
}

void FollowCameraComponent::Update(double curFrame, double deltaFrame)
{
	__super::Update(curFrame, deltaFrame);

	if (m_FollowNode != 0) {
		glm::vec3 position = glm::vec3(m_FollowNode->GetWorldTransform()[3]);

		m_Target = glm::normalize(position - m_Camera.GetPosition());
		m_ViewDistance = glm::length(position - m_Camera.GetPosition());
	}
}

void FollowCameraComponent::OnMouseMove(double x, double y)
{
	if (m_MousePress) {
		__super::OnMouseMove(x, y);

		float xOffset = x - m_MouseX;
		float yOffset = m_MouseY - y;
		m_MouseX = x;
		m_MouseY = y;

		xOffset *= m_MouseSensitivity * m_DeltaFrame;
		yOffset *= m_MouseSensitivity * m_DeltaFrame;

		m_Pitch += yOffset;
		if (m_Pitch > 87.0f)
		{
			m_Pitch = 87.0f;
		}
		else if (m_Pitch < -87.0f) {
			m_Pitch = -87.0f;
		}
		else {
			if (m_FollowNode != 0) {
				glm::vec3 nodePos = m_FollowNode->GetWorldTransform()[3];
				glm::mat4 matrix;
				matrix = glm::translate(matrix, nodePos);
				matrix = glm::rotate(matrix, glm::radians(-xOffset), glm::vec3(0.0f, 1.0f, 0.0f));
				matrix = glm::rotate(matrix, glm::radians(yOffset), glm::vec3(1.0f, 0.0f, 0.0f));
				matrix = glm::translate(matrix, -nodePos);
				m_Camera.SetPosition(matrix * glm::vec4(m_Camera.GetPosition(), 1.0f));
			}
		}
	}
}

void FollowCameraComponent::OnMouseScroll(double xOffset, double yOffset)
{
	float cameraSpeed = m_KeySensitivity * yOffset * 0.1f;
	if (yOffset > 0) {
		if (m_ViewDistance >= m_MinViewDistance) {
			m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
		}
	}
	else {
		m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
	}
}

void FollowCameraComponent::OnKeyboard(int key)
{
	float cameraSpeed = m_KeySensitivity * m_DeltaFrame;

	switch (key)
	{
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
	{
		if (m_ViewDistance >= m_MinViewDistance) {
			m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
		}
	}
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

Node* FollowCameraComponent::GetFollowNode()
{
	return m_FollowNode;
}

void FollowCameraComponent::SetFollowNode(Node* node)
{
	m_FollowNode = node;
	if (m_FollowNode != 0) {
		glm::vec3 nodePos = m_FollowNode->GetWorldTransform()[3];
		m_Camera.SetPosition(glm::vec3(nodePos.x, nodePos.y, 5.0f));
		m_Pitch = 0;
	}
}
