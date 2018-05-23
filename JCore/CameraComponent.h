#pragma once
#include "BaseComponent.h"
#include "Camera.h"

class World;
class CameraComponent : public BaseComponent
{
public:
	CameraComponent(int width, int height);
	~CameraComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual void Resize(int width, int height);

	virtual void OnKeyboard(int key);
	virtual void OnMouseUp();
	virtual void OnMouseDown();
	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset) = 0;

	glm::vec3&  GetPos();
	void SetPos(const glm::vec3& value);
	glm::vec3&  GetTarget();
	glm::vec3&  GetUp();
	float GetFov();

	glm::mat4 GetViewTransform();

	glm::mat4 GetProjectionTransform();

	bool GetIsActive();
	void SetIsActive(bool value);

	Camera* GetCamera();

	void SetWorld(World* world);
	World* GetWorld();

	float& GetKeySensitivity();

protected:
	float m_MouseX;
	float m_MouseY;
	float m_MouseSensitivity;
	float m_KeySensitivity;
	float m_CurFrame;
	float m_DeltaFrame;
	bool m_MousePress;
	glm::vec3 m_Target;
	glm::vec3 m_Up;
	bool m_IsActive;

	Camera m_Camera;

	World* m_World;
private:

	bool m_FirstMouse;
};

