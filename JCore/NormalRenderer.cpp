#include "stdafx.h"
#include "NormalRenderer.h"
#include "ResourceManager.h"

NormalRenderer::NormalRenderer()
{
}


NormalRenderer::~NormalRenderer()
{
}

void NormalRenderer::Render(Scene* scene, RenderContext* context)
{
	Shader shaderShowNormal = ResourceManager::getInstance()->GetShader("show_normal");
	shaderShowNormal.use();
	shaderShowNormal.setMatrix4("view", context->MatView);
	shaderShowNormal.setMatrix4("projection", context->MatProj);

	//渲染Model
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderShowNormal);
	}

	//渲染自定义图元
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderShowNormal);
	}
}
