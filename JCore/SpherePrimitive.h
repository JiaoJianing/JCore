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

	virtual void RenderDebug(Shader shader);

private:
	void initRenderData();
};

