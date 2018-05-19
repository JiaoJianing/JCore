#pragma once
#include "BaseRenderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Quad.h"

class PostEffectRenderer : public BaseRenderer
{
public:
	PostEffectRenderer();
	~PostEffectRenderer();

	virtual void Initialize(int width, int height);

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

	void BeginRender();

	void EndRender();

private:
	void initGraphicsRes(int width, int height);

	void deleteGraphicsRes();

private:
	unsigned int m_MSFBO, m_FBO;
	unsigned int m_RBO, m_DepthRBO;
	Texture m_Texture;
	Quad m_Quad;
};

