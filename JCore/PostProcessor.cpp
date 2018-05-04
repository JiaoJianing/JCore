#include "stdafx.h"
#include "PostProcessor.h"

float postVertices[] = {
	// Pos        // Tex
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,

	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f
};

PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
	: PostProcessShader(shader), Width(width), Height(height)
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
	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to initialize MS FBO" << std::endl;
	}

	//初始化全屏帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	this->Texture.Generate(width, height, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.GetID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to initialize FBO" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//初始化渲染数据和uniform数据
	this->initRenderData();
	this->PostProcessShader.use();
	this->PostProcessShader.setInt("scene", 0);
}

PostProcessor::~PostProcessor()
{
	glDeleteFramebuffers(1, &m_MSFBO);
	glDeleteFramebuffers(1, &m_FBO);

	glDeleteVertexArrays(1, &m_VAO);
}

void PostProcessor::BeginRender()
{
	//渲染到多重采样帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessor::EndRender()
{
	//将多重采样帧缓冲复制到 全屏帧缓冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	this->PostProcessShader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->Texture.GetID());
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(postVertices), postVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
