#pragma once
#include "Renderer.h"
#include "Shader.h"

struct PickInfo {
	float nodeID;
	float reserved1;
	float reserved2;
};

class PickRenderer : public Renderer
{
public:
	PickRenderer(int width, int height);
	~PickRenderer();

	virtual void Initialize();

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

	PickInfo Pick(unsigned int x, unsigned int y);

private:
	unsigned int m_FBO;
	unsigned int m_PickTexture;
	unsigned int m_DepthTexture;
	Shader m_PickShader;
};

