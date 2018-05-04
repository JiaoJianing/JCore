#pragma once
#include "Node.h"
#include "CameraComponent.h"

class FollowCameraComponent : public CameraComponent
{
public:
	FollowCameraComponent(int width, int height);
	~FollowCameraComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset);

	virtual void OnKeyboard(int key) override;

	Node* GetFollowNode();
	void SetFollowNode(Node* node);

private:
	Node * m_FollowNode;
	float m_ViewDistance;
	float m_MinViewDistance;
	float m_Pitch;
};

