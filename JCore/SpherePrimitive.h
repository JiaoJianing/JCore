#pragma once
#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include "CustomPrimitive.h"

class SpherePrimitive : public CustomPrimitive
{
public:
	SpherePrimitive();
	~SpherePrimitive();

	virtual void Render(Shader shader);

	virtual void RenderSimple(Shader shader);

	virtual void RenderBoundingBox(Shader shader);

private:
	void initRenderData();
};

