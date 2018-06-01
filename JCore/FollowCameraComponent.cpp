#include "stdafx.h"
#include "FollowCameraComponent.h"

FollowCameraComponent::FollowCameraComponent(int width, int height)
	: CameraComponent(width, height)
	, m_FollowNode(0)
	, m_ViewDistance(10)
	, m_Pitch(0)
	, m_Yaw(0)
	, m_FollowNodeSRT(0)
	, m_CamDirection(0.0f, -1.0f, 1.0f)
	, m_BaseCamDirection(0.0f, -1.0f, 1.0f)
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
		glm::vec3 position = m_FollowNodeSRT->GetTranslation();

		m_Target = glm::normalize(position - m_Camera.GetPosition());
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

		m_Yaw += xOffset;
		m_Pitch += yOffset;
		m_Pitch = __max(__min(m_Pitch, 35.0f), -35.0f);

		if (m_FollowNode != 0) {
			m_FollowNodeSRT->SetRotate(m_FollowNode->GetUpDir() * -m_Yaw);
			glm::mat4 cameraRotate;
			cameraRotate = glm::rotate(cameraRotate, glm::radians(m_Pitch), glm::normalize(glm::cross(m_FollowNode->GetFrontDir(), m_FollowNode->GetUpDir())));
			m_CamDirection = cameraRotate * glm::vec4(m_BaseCamDirection, 0.0f);

			updateCameraState(m_FollowNodeSRT->GetTranslation());
		}
	}
}

void FollowCameraComponent::OnMouseScroll(double xOffset, double yOffset)
{
	m_ViewDistance -= yOffset;
	m_ViewDistance = __max(__min(m_ViewDistance, 50), 3);
	if (m_FollowNodeSRT != 0) {
		updateCameraState(m_FollowNodeSRT->GetTranslation());
	}
}

void FollowCameraComponent::OnKeyboard(int key)
{
	float cameraSpeed = m_KeySensitivity * m_DeltaFrame;

	if (m_FollowNode != 0) {
		switch (key)
		{
		case GLFW_KEY_UP:
		case GLFW_KEY_W: {
			glm::vec3 nodePos = m_FollowNodeSRT->GetTranslation();
			glm::vec3 nodeFront = m_FollowNode->GetFrontDir();
			nodeFront = glm::normalize(m_FollowNode->GetWorldTransform() * glm::vec4(nodeFront, 0.0f));
			m_FollowNodeSRT->SetTranslation(m_FollowNodeSRT->GetTranslation() + nodeFront * cameraSpeed);

			updateCameraState(nodePos);
		}
						 break;
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S: {
			glm::vec3 nodePos = m_FollowNodeSRT->GetTranslation();
			glm::vec3 nodeFront = m_FollowNode->GetFrontDir();
			nodeFront = glm::normalize(m_FollowNode->GetWorldTransform() * glm::vec4(nodeFront, 0.0f));
			m_FollowNodeSRT->SetTranslation(m_FollowNodeSRT->GetTranslation() - nodeFront * cameraSpeed);

			updateCameraState(nodePos);
		}
						 break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A: {
			glm::vec3 nodePos = m_FollowNodeSRT->GetTranslation();
			glm::vec3 nodeFront = m_FollowNode->GetFrontDir();
			nodeFront = glm::normalize(m_FollowNode->GetWorldTransform() * glm::vec4(nodeFront, 0.0f));
			m_FollowNodeSRT->SetTranslation(m_FollowNodeSRT->GetTranslation() - glm::normalize(glm::cross(nodeFront, m_Up)) * cameraSpeed);

			updateCameraState(nodePos);
		}
						 break;
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D: {
			glm::vec3 nodePos = m_FollowNodeSRT->GetTranslation();
			glm::vec3 nodeFront = m_FollowNode->GetFrontDir();
			nodeFront = glm::normalize(m_FollowNode->GetWorldTransform() * glm::vec4(nodeFront, 0.0f));
			m_FollowNodeSRT->SetTranslation(m_FollowNodeSRT->GetTranslation() + glm::normalize(glm::cross(nodeFront, m_Up)) * cameraSpeed);

			updateCameraState(nodePos);
		}
						 break;
		default:
			break;
		}
	}
}

Node* FollowCameraComponent::GetFollowNode()
{
	return m_FollowNode;
}

void FollowCameraComponent::SetFollowNode(Node* node)
{
	m_FollowNode = node;
	m_FollowNodeSRT = 0;
	m_Pitch = 0;
	m_Yaw = 0;
	m_ViewDistance = 10;

	if (m_FollowNode != 0) {
		m_BaseCamDirection = m_CamDirection = -(-m_FollowNode->GetFrontDir() + m_FollowNode->GetUpDir());
		m_FollowNodeSRT = m_FollowNode->FindComponent<SRTTransformComponent>();
		updateCameraState(m_FollowNodeSRT->GetTranslation());
	}
}

void FollowCameraComponent::updateCameraState(const glm::vec3& nodePosition)
{
	if (m_FollowNode != 0) {
		glm::vec3 nodeDirection = glm::normalize(m_FollowNode->GetWorldTransform() * glm::vec4(m_CamDirection, 0.0f));
		glm::vec3 cameraPos = nodePosition - glm::vec3(nodeDirection) * m_ViewDistance;
		m_Camera.SetPosition(glm::vec3(cameraPos));
	}
}
