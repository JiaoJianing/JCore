#pragma once
#include "Scene.h"
#include "Renderer.h"

class NormalRenderer
{
public:
	NormalRenderer();
	~NormalRenderer();

	void Render(Scene* scene, RenderContext* context);
};

