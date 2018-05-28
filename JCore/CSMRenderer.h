#pragma once
#include "BaseRenderer.h"
#include <glm.hpp>

struct CascadeInfo {
	unsigned int ShadowTexture;
	glm::mat4 LightViewProj;
	glm::mat4 CameraProj;
	float CascadeSpace;
	float ShadowmapSize;
};

class CSMRenderer
{
public:
	CSMRenderer();
	~CSMRenderer();

	void Initialize(int width, int height);

	void Render(Scene* scene, RenderContext* context);

	void Resize(int width, int height);

	CascadeInfo* GetCascadeAt(int index);

private:
	void updateCascades(RenderContext* context);

private:
	unsigned int m_ShadowFBO;
	CascadeInfo m_Cascades[3];
};

