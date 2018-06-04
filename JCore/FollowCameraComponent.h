#pragma once
#include "Node.h"
#include "CameraComponent.h"
#include "SRTTransformComponent.h"

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
	void updateCameraState(const glm::vec3& nodePosition);

private:
	Node * m_FollowNode;
	float m_ViewDistance;
	float m_Pitch;
	float m_Yaw;
	SRTTransformComponent* m_FollowNodeSRT;
	glm::vec3 m_CamDirection;
	glm::vec3 m_BaseCamDirection;
	glm::vec3 m_CamRotDirection;
};

