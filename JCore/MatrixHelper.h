#pragma once
#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>

class MatrixHelper
{
public:
	MatrixHelper();
	~MatrixHelper();

	static glm::mat4 Convert2GlmMatrix(const aiMatrix4x4& mat);

	static glm::mat4 Convert2GlmMatrix(const aiMatrix3x3& mat);
};

