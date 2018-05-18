#pragma once
#include "Scene.h"
#include "CameraComponent.h"
#include "SilhouetteRenderer.h"
#include <glm.hpp>

class RenderContext {
public:
	glm::mat4 MatWorld;
	glm::mat4 MatView;
	glm::mat4 MatProj;
	glm::vec3 ViewPos;

	void GetParamsFromCamera(CameraComponent* camera);
	void UpdateTransform();
};

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	virtual void Initialize();

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

protected:
	int m_Width;
	int m_Height;
};

