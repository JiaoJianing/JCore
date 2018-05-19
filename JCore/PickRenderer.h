#pragma once
#include "BaseRenderer.h"
#include "Shader.h"

struct PickInfo {
	float nodeID;
	float reserved1;
	float reserved2;
};

class PickRenderer : public BaseRenderer
{
public:
	PickRenderer();
	~PickRenderer();

	virtual void Initialize(int width, int height);

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

	PickInfo Pick(unsigned int x, unsigned int y);

private:
	void initGraphicsRes(int width, int height);

	void deleteGraphicsRes();

private:
	unsigned int m_FBO;
	unsigned int m_PickTexture;
	unsigned int m_DepthTexture;
};

