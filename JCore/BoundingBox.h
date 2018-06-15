#pragma once
#include "Aabb.h"
#include "Shader.h"
#include <vector>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void Render(Shader shader);

	AABB& GetAABB();
	void SetAABB(const AABB& aabb);

private:
	void initialize();

private:
	unsigned int m_VAO, m_VBO;
	unsigned int m_EBO;
	AABB m_AABB;
	bool m_Init;
};

