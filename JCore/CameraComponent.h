#pragma once
#include "BaseComponent.h"

class CameraComponent : public BaseComponent
{
public:
	CameraComponent(int width, int height);
	~CameraComponent();

	virtual std::string GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void Render();

	virtual void Resize(int width, int height);

	virtual void OnKeyboard(int key);
	virtual void OnMouseUp();
	virtual void OnMouseDown();
	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset) = 0;

	const glm::vec3&  GetPos() const;
	void SetPos(const glm::vec3& value);
	const glm::vec3&  GetTarget() const;
	const glm::vec3&  GetUp() const;
	const float GetFov() const;

	glm::mat4 GetViewTransform();

	glm::mat4 GetProjectionTransform();

	bool GetIsActive();
	void SetIsActive(bool value);

protected:
	float m_MouseX;
	float m_MouseY;
	float m_MouseSensitivity;
	float m_KeySensitivity;
	float m_CurFrame;
	float m_DeltaFrame;
	int m_WindowWidth;
	int m_WindowHeight;
	bool m_MousePress;
	glm::vec3 m_Pos;
	glm::vec3 m_Target;
	glm::vec3 m_Up;
	float m_Fov;
	bool m_IsActive;

private:

	bool m_FirstMouse;
};

