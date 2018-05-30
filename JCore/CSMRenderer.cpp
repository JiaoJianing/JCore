#include "stdafx.h"
#include "CSMRenderer.h"
#include "Shader.h"
#include "ResourceManager.h"

CSMRenderer::CSMRenderer()
{
}


CSMRenderer::~CSMRenderer()
{
}

void CSMRenderer::Initialize(int width, int height)
{
	m_Cascades[0].ShadowmapSize = 2048;
	m_Cascades[1].ShadowmapSize = 2048;
	m_Cascades[2].ShadowmapSize = 1024;

	m_Cascades[0].CameraProj = glm::perspective(45.0f, (float)width / height, 0.1f, 30.0f);
	m_Cascades[1].CameraProj = glm::perspective(45.0f, (float)width / height, 20.0f, 80.0f);
	m_Cascades[2].CameraProj = glm::perspective(45.0f, (float)width / height, 60.0f, 400.0f);

	glGenFramebuffers(1, &m_ShadowFBO);
	for (unsigned int i = 0; i < 3; i++) {
		glGenTextures(1, &m_Cascades[i].ShadowTexture);
		glBindTexture(GL_TEXTURE_2D, m_Cascades[i].ShadowTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Cascades[i].ShadowmapSize, m_Cascades[i].ShadowmapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Cascades[0].ShadowTexture, 0);
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_SunDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
}

void CSMRenderer::Render(Scene* scene, RenderContext* context)
{
	updateCascades(context);

	Shader shaderCSM = ResourceManager::getInstance()->GetShader("csm");
	Shader shaderCSM_Animation = ResourceManager::getInstance()->GetShader("csm_animation");
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
	for (int i = 0; i < 3; i++) {
		glViewport(0, 0, m_Cascades[i].ShadowmapSize, m_Cascades[i].ShadowmapSize);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Cascades[i].ShadowTexture, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		//terrain
		shaderCSM.use();
		glm::mat4 model;
		shaderCSM.setMatrix4("model", model);
		shaderCSM.setMatrix4("lightViewProj", m_Cascades[i].LightViewProj);
		scene->GetTerrain()->Render(shaderCSM);

		for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
			(*it)->Render(shaderCSM);
		}

		for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
			(*it)->Render(shaderCSM);
		}

		shaderCSM_Animation.use();
		shaderCSM_Animation.setMatrix4("model", model);
		shaderCSM_Animation.setMatrix4("lightViewProj", m_Cascades[i].LightViewProj);		
		for (std::vector<Model*>::iterator it = scene->GetAnimationModels().begin(); it != scene->GetAnimationModels().end(); it++) {
			(*it)->Render(shaderCSM_Animation);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CSMRenderer::Resize(int width, int height)
{
	m_Cascades[0].CameraProj = glm::perspective(45.0f, (float)width / height, 0.1f, 30.0f);
	m_Cascades[1].CameraProj = glm::perspective(45.0f, (float)width / height, 25.0f, 80.0f);
	m_Cascades[2].CameraProj = glm::perspective(45.0f, (float)width / height, 75.0f, 400.0f);
}

CascadeInfo* CSMRenderer::GetCascadeAt(int index)
{
	if (index >= 3 || index < 0) {
		return 0;
	}

	return &m_Cascades[index];
}

glm::vec3& CSMRenderer::GetSunDirection()
{
	return m_SunDirection;
}

void CSMRenderer::SetSunDirection(const glm::vec3& value)
{
	m_SunDirection = value;
}

void CSMRenderer::updateCascades(RenderContext* context)
{
	glm::mat4 cameraView = glm::lookAt(context->ViewPos, context->ViewPos + context->ViewTarget, context->ViewUp);
	glm::vec4 ndcCoords[8] = {
		glm::vec4(1.0f, 1.0f, -1.0f, 1.0f), //top right near
		glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f), //top left near
		glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f), //bottom left near
		glm::vec4(1.0f, -1.0f, -1.0f, 1.0f), //bottom right near
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
	};

	for (int i = 0; i < 3; i++) {
		glm::mat4 cameraViewProj = m_Cascades[i].CameraProj * cameraView;
		glm::mat4 cameraInvViewProj = glm::inverse(cameraViewProj);
	
		glm::vec3 corners[8];
		for (int i = 0; i < 8; i++) {
			glm::vec4 corner = cameraInvViewProj * ndcCoords[i];
			corner.w = 1.0f / corner.w;
			corner.x *= corner.w;
			corner.y *= corner.w;
			corner.z *= corner.w;
			corners[i] = corner;
		}

		glm::vec3 frustumCenter;
		for (int j = 0; j < 8; j++) {
			frustumCenter = frustumCenter + corners[j];
		}
		frustumCenter = frustumCenter * (1.0f / 8.0f);

		float radius = glm::length(corners[1] - corners[7]) / 2.0f;
		float texelsPerUnit = m_Cascades[i].ShadowmapSize / (radius * 2.0f);
		glm::mat4 matScale;
		matScale = glm::scale(matScale, glm::vec3(texelsPerUnit));
		glm::mat4 matView = glm::lookAt(-m_SunDirection, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matView *= matScale;
		glm::mat4 matInvView = glm::inverse(matView);

		frustumCenter = matView * glm::vec4(frustumCenter, 1.0f);
		frustumCenter.x = (float)floor(frustumCenter.x);
		frustumCenter.y = (float)floor(frustumCenter.y);
		frustumCenter = matInvView * glm::vec4(frustumCenter, 1.0f);

		glm::vec3 eye = frustumCenter - (m_SunDirection * radius * 2.0f);
		glm::mat4 lookat = glm::lookAt(eye, frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::ortho(-radius, radius, -radius, radius, -6 * radius, 6 * radius);

		m_Cascades[i].LightViewProj = proj * lookat;
		m_Cascades[i].CascadeSpace = radius * 2.0f;
	}
}
