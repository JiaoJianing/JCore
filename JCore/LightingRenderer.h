#pragma once
#include "Renderer.h"
#include "CubePrimitive.h"

class LightingRenderer : public Renderer
{
public:
	LightingRenderer(int width, int height);
	~LightingRenderer();

	virtual void Initialize() override;

	virtual void Render(Scene* scene, RenderContext* context) override;

	virtual void Resize(int width, int height) override;

private:
	//������Դλ�á���������
	void RenderLightDebug(Scene* scene, RenderContext* context);
	CubePrimitive m_CubeDebug;
};

