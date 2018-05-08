#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	: m_Position(0.0f, 0.0f, 3.0f)
	, m_Fov(45.0f)
	, m_Aspect(1.0f)
	, m_Near(0.1)
	, m_Far(1000)
	, m_ViewportRect(0.0f, 0.0f, 800.0f, 600.0f)
	, m_IsActive(false)
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(const glm::vec3& value)
{
	if (m_Position != value) {
		m_Position = value;
	}
}

glm::vec3& Camera::GetPosition()
{
	return m_Position;
}

void Camera::SetViewTransform(const glm::mat4& value)
{
	if (m_ViewTransform != value) {
		m_ViewTransform = value;
	}
}

glm::mat4& Camera::GetViewTransform()
{
	return m_ViewTransform;
}

void Camera::SetProjTransform(const glm::mat4& value)
{
	if (m_ProjTransform != value) {
		m_ProjTransform = value;
	}
}

glm::mat4& Camera::GetProjTransform()
{
	return m_ProjTransform;
}

void Camera::SetViewProjTransform(const glm::mat4& value)
{
	if (m_ViewProjTransform != value) {
		m_ViewProjTransform = value;
	}
}

glm::mat4& Camera::GetViewProjTransform()
{
	return m_ViewProjTransform;
}

void Camera::SetFov(float value)
{
	//todo ¾«¶È
	if (m_Fov != value) {
		m_Fov = value;
	}
}

float Camera::GetFov()
{
	return m_Fov;
}

void Camera::SetAspect(float value)
{
	if (m_Aspect != value) {
		m_Aspect = value;
	}
}

float Camera::GetAspect()
{
	return m_Aspect;
}

void Camera::SetViewportRect(const glm::vec4& value)
{
	if (m_ViewportRect != value) {
		m_ViewportRect = value;
	}
}

glm::vec4& Camera::GetViewportRect()
{
	return m_ViewportRect;
}

void Camera::SetNear(float value)
{
	if (m_Near != value) {
		m_Near = value;
	}
}

float Camera::GetNear()
{
	return m_Near;
}

void Camera::SetFar(float value)
{
	if (m_Far != value) {
		m_Far = value;
	}
}

float Camera::GetFar()
{
	return m_Far;
}

void Camera::SetIsActive(float value)
{
	m_IsActive = value;
}

bool Camera::GetIsActive()
{
	return m_IsActive;
}
