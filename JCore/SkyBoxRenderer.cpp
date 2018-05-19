#include "stdafx.h"
#include "SkyBoxRenderer.h"
#include "ResourceManager.h"

SkyBoxRenderer::SkyBoxRenderer(int width, int height)
	: BaseRenderer(width, height)
{
}


SkyBoxRenderer::~SkyBoxRenderer()
{
}

void SkyBoxRenderer::Initialize()
{
}

void SkyBoxRenderer::Render(Scene* scene, RenderContext* context)
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	//glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	Shader shaderSkybox = ResourceManager::getInstance()->GetShader("skybox");
	Texture* textureSkybox = ResourceManager::getInstance()->GetTexture("skybox2");
	shaderSkybox.use();
	shaderSkybox.setMatrix4("view", context->MatView);
	shaderSkybox.setMatrix4("projection", context->MatProj);
	glm::mat4 model;
	model = glm::translate(model, context->ViewPos);
	shaderSkybox.setMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox->GetID());

	m_Sphere.RenderSimple(shaderSkybox);

	//glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}

void SkyBoxRenderer::Resize(int width, int height)
{
}
