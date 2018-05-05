#pragma once
#include "Shader.h"

class Quad
{
public:
	Quad();
	~Quad();

	void Draw(Shader shader);

private:
	unsigned int m_VAO, m_VBO;
};

