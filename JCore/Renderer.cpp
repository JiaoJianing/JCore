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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//äÖÈ¾ÂÖÀª
	m_SilhouetteRenderer.Render(scene, context);

	//äÖÈ¾Model
	Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
	shaderModel.use();
	shaderModel.setMatrix4("view", context->MatView);
	shaderModel.setMatrix4("projection", context->MatProj);
	shaderModel.setVec3("viewPos", context->ViewPos);
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//äÖÈ¾×Ô¶¨ÒåÍ¼Ôª
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderModel);
	}

	//äÖÈ¾Billboard
	Shader shaderBillboard = ResourceManager::getInstance()->GetShader("billboard");
	shaderBillboard.use();
	shaderBillboard.setMatrix4("view", context->MatView);
	shaderBillboard.setMatrix4("projection", context->MatProj);
	shaderBillboard.setVec3("viewPos", context->ViewPos);
	for (std::vector<Billboard*>::iterator it = scene->GetBillboards().begin(); it != scene->GetBillboards().end(); it++) {
		(*it)->Render(shaderBillboard);
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
