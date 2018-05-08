#include "stdafx.h"
#include "CameraComponent.h"
#include "World.h"

CameraComponent::CameraComponent(int width, int height)
	: m_Target(0.0f, 0.0f, -1.0f)
	, m_Up(0.0f, 1.0f, 0.0f)
	, m_CurFrame(0)
	, m_DeltaFrame(0)
	, m_MouseX(0)
	, m_MouseY(0)
	, m_MouseSensitivity(5)
	, m_KeySensitivity(5)
	, m_MousePress(false)
	, m_FirstMouse(true)
	, m_IsActive(false)
{
	m_Camera.SetViewportRect(glm::vec4(0.0f, 0.0f, width, height));
	m_Camera.SetAspect((float)width / height);
	m_Camera.SetProjTransform(glm::perspective(m_Camera.GetFov(), m_Camera.GetAspect(), m_Camera.GetNear(), m_Camera.GetFar()));
	m_Camera.SetViewTransform(glm::lookAt(m_Camera.GetPosition(), m_Camera.GetPosition() + GetTarget(), GetUp()));
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

	m_Camera.SetIsActive(GetIsActive());
}

void CameraComponent::Resize(int width, int height)
{
	m_Camera.SetViewportRect(glm::vec4(0.0f, 0.0f, width, height));
	m_Camera.SetAspect((float)width / height);
	m_Camera.SetProjTransform(glm::perspective(m_Camera.GetFov(), m_Camera.GetAspect(), m_Camera.GetNear(), m_Camera.GetFar()));
}

void CameraComponent::OnKeyboard(int key)
{
	float cameraSpeed = m_KeySensitivity * m_DeltaFrame;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_Camera.SetPosition(m_Camera.GetPosition() + cameraSpeed * m_Target);
		break;
	case GLFW_KEY_DOWN:
		m_Camera.SetPosition(m_Camera.GetPosition() - cameraSpeed * m_Target);
		break;
	case GLFW_KEY_LEFT:
		m_Camera.SetPosition(m_Camera.GetPosition() - glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed);
		break;
	case GLFW_KEY_RIGHT:
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::normalize(glm::cross(m_Target, m_Up)) * cameraSpeed);
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

void CameraComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetCameras().push_back(&m_Camera);
}

void CameraComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetCameras().begin(), world->GetScene()->GetCameras().end(), &m_Camera);
	if (it != world->GetScene()->GetCameras().end()) {
		world->GetScene()->GetCameras().erase(it);
	}
}

glm::vec3& CameraComponent::GetPos()
{
	return m_Camera.GetPosition();
}

void CameraComponent::SetPos(const glm::vec3& value)
{
	m_Camera.SetPosition(value);
}

glm::vec3& CameraComponent::GetTarget()
{
	return m_Target;
}

glm::vec3& CameraComponent::GetUp()
{
	return m_Up;
}

float CameraComponent::GetFov()
{
	return m_Camera.GetFov();
}

glm::mat4 CameraComponent::GetViewTransform()
{
	m_Camera.SetViewTransform(glm::lookAt(m_Camera.GetPosition(), m_Camera.GetPosition() + GetTarget(), GetUp()));
	return m_Camera.GetViewTransform();
}

glm::mat4 CameraComponent::GetProjectionTransform()
{
	return m_Camera.GetProjTransform();
}

bool CameraComponent::GetIsActive()
{
	return m_IsActive;
}

void CameraComponent::SetIsActive(bool value)
{
	m_IsActive = value;
}

Camera* CameraComponent::GetCamera()
{
	return &m_Camera;
}
