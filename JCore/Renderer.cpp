#include "stdafx.h"
#include "Renderer.h"
#include "Model.h"
#include "ResourceManager.h"
#include "CustomPrimitive.h"

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
	//渲染Model
	Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
	shaderModel.use();
	shaderModel.setMatrix4("view", context->MatView);
	shaderModel.setMatrix4("projection", context->MatProj);
	shaderModel.setVec3("viewPos", context->ViewPos);
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//渲染自定义图元
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//渲染Billboard
	Shader shaderBillboard = ResourceManager::getInstance()->GetShader("billboard");
	shaderBillboard.use();
	shaderBillboard.setMatrix4("view", context->MatView);
	shaderBillboard.setMatrix4("projection", context->MatProj);
	shaderBillboard.setVec3("viewPos", context->ViewPos);
	for (std::vector<Billboard*>::iterator it = scene->GetBillboards().begin(); it != scene->GetBillboards().end(); it++) {
		(*it)->Render(shaderBillboard);
	}

	//渲染粒子系统
	Shader shaderParticleSys = ResourceManager::getInstance()->GetShader("particle_system");
	shaderParticleSys.use();
	shaderParticleSys.setMatrix4("view", context->MatView);
	shaderParticleSys.setMatrix4("projection", context->MatProj);
	shaderParticleSys.setVec3("viewPos", context->ViewPos);
	for (std::vector<ParticleSystem*>::iterator it = scene->GetParticleSystems().begin(); it != scene->GetParticleSystems().end(); it++) {
		(*it)->Render(shaderParticleSys);
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
