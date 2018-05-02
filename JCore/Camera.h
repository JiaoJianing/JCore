#pragma once
#include <glm.hpp>

class Camera
{
public:
	Camera(float windowWidth, float windowHeight);
	Camera(float windowWidth, float windowHeight, const glm::vec3& pos, const glm::vec3&  target, const glm::vec3& up);
	~Camera();

	const glm::vec3&  GetPos() const;
	const glm::vec3&  GetTarget() const;
	const glm::vec3&  GetUp() const;
	const float GetFov() const;

	void Update(float curFrame, float deltaFrame);	
	
	void OnKeyboard(int key);
	void OnMouseMove(double x, double y);
	void OnMouseUp();
	void OnMouseDown();
	void OnMouseScroll(double xOffset, double yOffset);

private:
	glm::vec3 m_pos;
	glm::vec3 m_target;
	glm::vec3 m_up;

	float m_curFrame;
	float m_deltaFrame;

	bool m_firstMouse;
	float m_mouseX;
	float m_mouseY;
	float m_mouseSensitivity;
	float m_keySensitivity;
	bool m_mousePress;

	float m_windowWidth;
	float m_windowHeight;

	float m_yaw;
	float m_pitch;
	float m_roll;

	float m_fov;
};

