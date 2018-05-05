#include "stdafx.h"
#include "PickingSystem.h"

PickingSystem::PickingSystem(int windowWidth, int windowHeight)
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	//第一个附件 记录绘制节点的id
	glGenTextures(1, &m_PickTexture);
	glBindTexture(GL_TEXTURE_2D, m_PickTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, 0);
	//设置纹理环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PickTexture, 0);
	//第二个附件 记录深度
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to init FBO in picking system!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickingSystem::~PickingSystem()
{
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteTextures(1, &m_PickTexture);
	glDeleteTextures(1, &m_DepthTexture);
}

void PickingSystem::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PickingSystem::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickInfo PickingSystem::Pick(unsigned int x, unsigned int y)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	PickInfo pick;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pick);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	return pick;
}
