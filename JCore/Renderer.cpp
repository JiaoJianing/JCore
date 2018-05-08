#include "stdafx.h"
#include "Renderer.h"
#include "Model.h"
#include "Cube.h"
#include "Sphere.h"
#include "ResourceManager.h"

Renderer::Renderer(int width, int height)
	: m_Width(width)
	, m_Height(height)
{
}


Renderer::~Renderer()
{
}

void Renderer::Initialize()
{

}

void Renderer::Render(Scene* scene, RenderContext* context)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//‰÷»æ¬÷¿™
	m_SilhouetteRenderer.Render(scene, context);

	//‰÷»æModel
	Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
	shaderModel.use();
	shaderModel.setMatrix4("view", context->MatView);
	shaderModel.setMatrix4("projection", context->MatProj);
	shaderModel.setVec3("viewPos", context->ViewPos);
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//‰÷»æCube
	Shader shaderCustom = ResourceManager::getInstance()->GetShader("custom");
	shaderCustom.use();
	shaderCustom.setMatrix4("view", context->MatView);
	shaderCustom.setMatrix4("projection", context->MatProj);
	shaderCustom.setVec3("viewPos", context->ViewPos);
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		(*it)->Render(shaderCustom);
	}

	//‰÷»æSphere
	for (std::vector<Sphere*>::iterator it = scene->GetSpheres().begin(); it != scene->GetSpheres().end(); it++) {
		(*it)->Render(shaderCustom);
	}
}

void Renderer::Resize(int width, int height)
{

}

void RenderContext::GetParamsFromCamera(CameraComponent* camera)
{
	MatView = camera->GetViewTransform();
	MatProj = camera->GetProjectionTransform();
	ViewPos = camera->GetPos();
}

void RenderContext::UpdateTransform()
{

}
