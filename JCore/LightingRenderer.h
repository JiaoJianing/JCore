#pragma once
#include "BaseRenderer.h"
#include "CubePrimitive.h"

class LightingRenderer : public BaseRenderer
{
public:
	LightingRenderer(int width, int height);
	~LightingRenderer();

	virtual void Initialize();

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

private:
	//������Դλ�á���������
	void RenderLightDebug(Scene* scene, RenderContext* context);
	CubePrimitive m_CubeDebug;
};

