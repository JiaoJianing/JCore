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

void Renderer::Render(Scene* scene)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//äÖÈ¾Model
	Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
	shaderModel.use();
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//äÖÈ¾Cube
	Shader shaderCustom = ResourceManager::getInstance()->GetShader("custom");
	shaderCustom.use();
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		(*it)->Render(shaderCustom);
	}

	//äÖÈ¾Sphere
	for (std::vector<Sphere*>::iterator it = scene->GetSpheres().begin(); it != scene->GetSpheres().end(); it++) {
		(*it)->Render(shaderCustom);
	}
}

void Renderer::Resize(int width, int height)
{

}
