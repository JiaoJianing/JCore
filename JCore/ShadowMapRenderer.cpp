#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "ResourceManager.h"
#include "DirLight.h"

ShadowMapRenderer::ShadowMapRenderer()
	: m_ShadowWidth(1024)
	, m_ShadowHeight(1024)
{
}


ShadowMapRenderer::~ShadowMapRenderer()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_ShadowMapTexture);
}

void ShadowMapRenderer::Initialize(int width, int height)
{
	glGenFramebuffers(1, &m_FBO);
	glGenTextures(1, &m_ShadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Failed to create Shadow Map FBO");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
	m_LightSpaceProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	m_LightSpaceMat = m_LightSpaceProj * view;
}

void ShadowMapRenderer::Render(Scene* scene, RenderContext* context)
{
	glViewport(0, 0, m_ShadowWidth, m_ShadowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (std::vector<BaseLight*>::iterator it = scene->GetLights().begin(); it != scene->GetLights().end(); it++) {
		if (dynamic_cast<DirLight*>(*it) != 0) {
			glm::mat4 view = glm::lookAt((*it)->GetLightPos(), glm::vec3(0.0f, 27.0f, 0.0f), glm::vec3(0, 1, 0));
			m_LightSpaceMat = m_LightSpaceProj * view;
		}
	}

	Shader shaderShadowMap = ResourceManager::getInstance()->GetShader("shadow_map");
	shaderShadowMap.use();
	shaderShadowMap.setMatrix4("lightSpaceMat", m_LightSpaceMat);

	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderShadowMap);
	}

	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderShadowMap);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapRenderer::Resize(int width, int height)
{

}

unsigned int ShadowMapRenderer::GetShadowMapTexture()
{
	return m_ShadowMapTexture;
}

glm::mat4& ShadowMapRenderer::GetLightSpaceMat()
{
	return m_LightSpaceMat;
}
