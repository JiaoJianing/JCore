#include "stdafx.h"
#include "PickRenderer.h"
#include "ResourceManager.h"

PickRenderer::PickRenderer()
{
}


PickRenderer::~PickRenderer()
{
	deleteGraphicsRes();
}

void PickRenderer::Initialize(int width, int height)
{
	__super::Initialize(width, height);
	initGraphicsRes(width, height);
}

void PickRenderer::Render(Scene* scene, RenderContext* context)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	Shader shaderPick = ResourceManager::getInstance()->GetShader("pick");
	shaderPick.use();
	shaderPick.setMatrix4("view", context->MatView);
	shaderPick.setMatrix4("projection", context->MatProj);
	shaderPick.setMatrix4("model", glm::mat4());
	//渲染Model
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderPick);
	}

	//渲染自定义图元
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderPick);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickRenderer::Resize(int width, int height)
{
	__super::Resize(width, height);
	deleteGraphicsRes();
	initGraphicsRes(width, height);
}

PickInfo PickRenderer::Pick(unsigned int x, unsigned int y)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	PickInfo pick;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pick);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return pick;
}

void PickRenderer::initGraphicsRes(int width, int height)
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	//第一个附件 记录绘制节点的id
	glGenTextures(1, &m_PickTexture);
	glBindTexture(GL_TEXTURE_2D, m_PickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, 0);
	//设置纹理环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PickTexture, 0);
	//第二个附件 记录深度
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to init FBO in picking system!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickRenderer::deleteGraphicsRes()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_PickTexture);
	glDeleteTextures(1, &m_DepthTexture);
}
