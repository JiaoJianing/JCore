#pragma once
#include "Scene.h"

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	virtual void Initialize();

	virtual void Render(Scene* scene);

	virtual void Resize(int width, int height);

protected:
	int m_Width;
	int m_Height;
};

