#pragma once
#include "Scene.h"
#include "CameraComponent.h"

class RenderContext {
public:
	glm::mat4 MatWorld;
	glm::mat4 MatView;
	glm::mat4 MatProj;
	glm::vec3 ViewPos;

	void GetParamsFromCamera(CameraComponent* camera);
	void UpdateTransform();
};

class BaseRenderer
{
public:
	BaseRenderer();
	~BaseRenderer();

	virtual void Initialize(int width, int height);

	virtual void Render(Scene* scene, RenderContext* context) = 0;

	virtual void Resize(int width, int height);

protected:
	int m_Width;
	int m_Height;
};

