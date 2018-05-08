#pragma once
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

class PostEffectRenderer : public Renderer
{
public:
	PostEffectRenderer(int width, int height);
	~PostEffectRenderer();

	virtual void Initialize();

	virtual void Render(Scene* scene);

	virtual void Resize(int width, int height);

	void BeginRender();

	void EndRender();

private:
	unsigned int m_MSFBO, m_FBO;
	unsigned int m_RBO;
	unsigned int m_VAO, m_VBO;
	Shader m_PostShader;
	Texture m_Texture;
};

