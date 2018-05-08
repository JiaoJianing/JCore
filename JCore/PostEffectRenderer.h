#pragma once
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Quad.h"

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
	Shader m_PostShader;
	Texture m_Texture;
	Quad m_Quad;
};

