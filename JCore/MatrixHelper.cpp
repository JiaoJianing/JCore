#include "stdafx.h"
#include "MatrixHelper.h"

MatrixHelper::MatrixHelper()
{
}


MatrixHelper::~MatrixHelper()
{
}

glm::mat4 MatrixHelper::Convert2GlmMatrix(const aiMatrix4x4& mat)
{
	return glm::mat4(mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4);
}

glm::mat4 MatrixHelper::Convert2GlmMatrix(const aiMatrix3x3& mat)
{
	return glm::mat4(mat.a1, mat.b1, mat.c1, 0.0f,
		mat.a2, mat.b2, mat.c2, 0.0f,
		mat.a3, mat.b3, mat.c3, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
