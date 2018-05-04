#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent(int width, int height)
	: m_Pos(0.0f, 0.0f, 3.0f)
	, m_Target(0.0f, 0.0f, -1.0f)
	, m_Up(0.0f, 1.0f, 0.0f)
	, m_CurFrame(0)
	, m_DeltaFrame(0)
	, m_MouseX(0)
	, m_MouseY(0)
	, m_MouseSensitivity(5)
	, m_KeySensitivity(5)
	, m_MousePress(false)
	, m_WindowWidth(width)
	, m_WindowHeight(height)
	, m_Fov(45.0f)
	, m_FirstMouse(true)
	, m_IsActive(false)
{
}


CameraComponent::~CameraComponent()
{
}

stringT CameraComponent::GetTypeName()
{
	return _T("CameraComponent");
}

void CameraComponent::Update(double curFrame, double deltaFrame)
{
	m_CurFrame = curFrame;
	m_DeltaFrame = deltaFrame;
}

void CameraComponent::Render()
{
}

void CameraComponent::Resize(int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void CameraComponent::OnKeyboard(int key)
{
	float cameraSpeed = m_KeySensitivity * m_DeltaFrame;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_Pos += cameraSpeed * m_Target;
		break;
	case GLFW_KEY_DOWN:
		m_Pos -= cameraSpeed * m_Target;
		break;
	case GLFW_KEY_LEFT:
		m_Pos -= glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed;
		break;
	case GLFW_KEY_RIGHT:
		m_Pos += glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed;
		break;
	default:
		break;
	}
}

void CameraComponent::OnMouseUp()
{
	m_MousePress = false;
	m_FirstMouse = true;
}

void CameraComponent::OnMouseDown()
{
	m_MousePress = true;
}

void CameraComponent::OnMouseMove(double x, double y)
{
	if (m_FirstMouse) {
		m_MouseX = x;
		m_MouseY = y;
		m_FirstMouse = false;
	}
}

const glm::vec3& CameraComponent::GetPos() const
{
	return m_Pos;
}

void CameraComponent::SetPos(const glm::vec3& value)
{
	m_Pos = value;
}

const glm::vec3& CameraComponent::GetTarget() const
{
	return m_Target;
}

const glm::vec3& CameraComponent::GetUp() const
{
	return m_Up;
}

const float CameraComponent::GetFov() const
{
	return m_Fov;
}

glm::mat4 CameraComponent::GetViewTransform()
{
	return glm::lookAt(GetPos(), GetPos() + GetTarget(), GetUp());
}

glm::mat4 CameraComponent::GetProjectionTransform()
{
	return glm::perspective(GetFov(), (float)m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
}

bool CameraComponent::GetIsActive()
{
	return m_IsActive;
}

void CameraComponent::SetIsActive(bool value)
{
	m_IsActive = value;
}
