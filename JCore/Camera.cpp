#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>


Camera::Camera(float windowWidth, float windowHeight)
{
	m_pos = glm::vec3(0, 0, 3);
	m_target = glm::vec3(0, 0, -1);
	m_up = glm::vec3(0, 1, 0);
	m_curFrame = 0;
	m_deltaFrame = 0;
	m_mouseX = m_mouseY = 0;
	m_firstMouse = true;
	m_mouseSensitivity = 5;
	m_keySensitivity = 5;
	m_mousePress = false;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_yaw = -90.0f;
	m_pitch = m_roll = 0;
	m_fov = 45.0f;
}


Camera::Camera(float windowWidth, float windowHeight, const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
	m_pos = pos;
	m_target = target;
	glm::normalize(m_target);
	m_up = up;
	glm::normalize(m_up);
	m_curFrame = 0;
	m_deltaFrame = 0;
	m_mouseX = m_mouseY = 0;
	m_firstMouse = true;
	m_mousePress = false;
	m_mouseSensitivity = 5;
	m_keySensitivity = 5;
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_yaw = -90.0f;
	m_pitch = m_roll = 0;
	m_fov = 45.0f;
}

Camera::~Camera()
{
}

void Camera::OnKeyboard(int key)
{
	float cameraSpeed = m_keySensitivity * m_deltaFrame;

	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		m_pos += cameraSpeed * m_target;
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		m_pos -= cameraSpeed * m_target;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		m_pos -= glm::normalize(glm::cross(m_target, m_up)) * cameraSpeed;
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		m_pos += glm::normalize(glm::cross(m_target, m_up)) * cameraSpeed;
		break;
	default:
		break;
	}
}

void Camera::OnMouseMove(double x, double y)
{
	if (m_mousePress) {
		if (m_firstMouse) {
			m_mouseX = x;
			m_mouseY = y;
			m_firstMouse = false;
		}

		float xOffset = x - m_mouseX;
		float yOffset = m_mouseY - y;
		m_mouseX = x;
		m_mouseY = y;

		xOffset *= m_mouseSensitivity * m_deltaFrame;
		yOffset *= m_mouseSensitivity * m_deltaFrame;

		m_yaw += xOffset;
		m_pitch += yOffset;
		if (m_pitch > 89.0f) m_pitch = 89.0f;
		if (m_pitch < -89.0f) m_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		front.y = sin(glm::radians(m_pitch));
		front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
		m_target = glm::normalize(front);
	}
}

void Camera::OnMouseUp()
{
	m_mousePress = false;
	m_firstMouse = true;
}

void Camera::OnMouseDown()
{
	m_mousePress = true;
}

void Camera::OnMouseScroll(double xOffset, double yOffset)
{
	float cameraSpeed = m_keySensitivity * yOffset * 0.1f;

	m_pos += cameraSpeed * m_target;
}

const glm::vec3& Camera::GetPos() const
{
	return m_pos;
}

const glm::vec3& Camera::GetTarget() const
{
	return m_target;
}

const glm::vec3& Camera::GetUp() const
{
	return m_up;
}

const float Camera::GetFov() const
{
	return m_fov;
}

void Camera::Update(float curFrame, float deltaFrame)
{
	m_curFrame = curFrame;
	m_deltaFrame = deltaFrame;
}
