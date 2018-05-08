#pragma once
#include <glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const glm::vec3& value);
	glm::vec3& GetPosition();

	void SetViewTransform(const glm::mat4& value);
	glm::mat4& GetViewTransform();

	void SetProjTransform(const glm::mat4& value);
	glm::mat4& GetProjTransform();

	void SetViewProjTransform(const glm::mat4& value);
	glm::mat4& GetViewProjTransform();

	void SetFov(float value);
	float GetFov();

	void SetAspect(float value);
	float GetAspect();

	void SetViewportRect(const glm::vec4& value);
	glm::vec4& GetViewportRect();

	void SetNear(float value);
	float GetNear();

	void SetFar(float value);
	float GetFar();

	void SetIsActive(float value);
	bool GetIsActive();

private:
	glm::vec3 m_Position;
	glm::mat4 m_ViewTransform;
	glm::mat4 m_ProjTransform;
	glm::mat4 m_ViewProjTransform;
	float m_Fov;
	float m_Aspect;
	glm::vec4 m_ViewportRect;
	float m_Near;
	float m_Far;
	bool m_IsActive;
};

