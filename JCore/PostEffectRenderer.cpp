#include "stdafx.h"
#include "PostEffectRenderer.h"
#include "ResourceManager.h"

PostEffectRenderer::PostEffectRenderer()
{
}


PostEffectRenderer::~PostEffectRenderer()
{
	deleteGraphicsRes();
}

void PostEffectRenderer::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	initGraphicsRes(width, height);
}

void PostEffectRenderer::Render(Scene* scene, RenderContext* context)
{
	Shader shaderPost = ResourceManager::getInstance()->GetShader("post");
	shaderPost.use();
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture.GetID());
	m_Quad.Render(shaderPost);
}

void PostEffectRenderer::Resize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	deleteGraphicsRes();
	initGraphicsRes(width, height);
}

void PostEffectRenderer::BeginRender()
{
	//渲染到多重采样帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostEffectRenderer::EndRender()
{
	//将多重采样帧缓冲复制到 全屏帧缓冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostEffectRenderer::initGraphicsRes(int width, int height)
{
	glGenFramebuffers(1, &m_MSFBO);
	glGenFramebuffers(1, &m_FBO);
	glGenRenderbuffers(1, &m_RBO);

	//初始化多重采样帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO);
	//深度附件
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to initialize MS FBO" << std::endl;
	}

	//初始化全屏帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	m_Texture.Generate(width, height, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.GetID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to initialize FBO" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostEffectRenderer::deleteGraphicsRes()
{
	glDeleteFramebuffers(1, &m_MSFBO);
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RBO);
	glDeleteRenderbuffers(1, &m_DepthRBO);
}
