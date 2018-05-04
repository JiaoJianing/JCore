#pragma once
#include "Shader.h"
#include "Texture.h"

class PostProcessor
{
public:
	PostProcessor(Shader shader, unsigned int width, unsigned int height);
	~PostProcessor();

	void BeginRender();

	void EndRender();

	void Render();

	Shader PostProcessShader;
	Texture Texture;
	unsigned int Width, Height;

private:
	unsigned int m_MSFBO, m_FBO;
	unsigned int m_RBO;
	unsigned int m_VAO, m_VBO;

	void initRenderData();
};

