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
	//äÖÈ¾Model
	Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
	shaderModel.use();
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//äÖÈ¾Cube
	Shader shaderCube = ResourceManager::getInstance()->GetShader("cube");
	shaderCube.use();
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		(*it)->Render(shaderCube);
	}

	//äÖÈ¾Sphere
	for (std::vector<Sphere*>::iterator it = scene->GetSpheres().begin(); it != scene->GetSpheres().end(); it++) {

	}
}

void Renderer::Resize(int width, int height)
{

}
