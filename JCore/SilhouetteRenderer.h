#pragma once
#include "Scene.h"

class RenderContext;
class SilhouetteRenderer
{
public:
	SilhouetteRenderer();
	~SilhouetteRenderer();

	void Render(Scene* scene, RenderContext* context);

	void BeginRender();

	void EndRender();
};

