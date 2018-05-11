#pragma once
#include "Renderer.h"
#include "SpherePrimitive.h"

class SkyBoxRenderer : public Renderer
{
public:
	SkyBoxRenderer(int width, int height);
	~SkyBoxRenderer();

	virtual void Initialize();

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

private:
	SpherePrimitive m_Sphere;
};

