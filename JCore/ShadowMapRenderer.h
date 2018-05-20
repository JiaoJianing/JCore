#pragma once
#include "BaseRenderer.h"

class ShadowMapRenderer
{
public:
	ShadowMapRenderer();
	~ShadowMapRenderer();

	void Initialize(int width, int height);

	void Render(Scene* scene, RenderContext* context);

	void Resize(int width, int height);

	unsigned int GetShadowMapTexture();

	glm::mat4& GetLightSpaceMat();

private:
	unsigned int m_FBO;
	unsigned int m_ShadowMapTexture;
	glm::mat4 m_LightSpaceMat;
	glm::mat4 m_LightSpaceProj;
	int m_ShadowWidth;
	int m_ShadowHeight;
};

