#pragma once
#include <glm.hpp>
#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include "CustomPrimitive.h"

class CubePrimitive : public CustomPrimitive{
public:
	CubePrimitive();
	~CubePrimitive();

	virtual void Render(Shader shader);

	virtual void RenderSimple(Shader shader);

private:
	void initRenderData();
};