#pragma once
#include "Scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Initialize();

	void Render(Scene* scene);

	void Render(Scene* scene, Shader shader);

	void Resize(int width, int height);
};

