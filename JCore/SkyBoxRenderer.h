#pragma once
#include "BaseRenderer.h"
#include "SpherePrimitive.h"
#include "CubePrimitive.h"

class SkyBoxRenderer : public BaseRenderer
{
public:
	SkyBoxRenderer(int width, int height);
	~SkyBoxRenderer();

	virtual void Initialize();

	virtual void Render(Scene* scene, RenderContext* context);

	virtual void Resize(int width, int height);

private:
	SpherePrimitive m_Sphere; //skydom
	//CubePrimitive m_Sphere; //skybox
};

