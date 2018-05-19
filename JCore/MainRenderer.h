#pragma once
#include "BaseRenderer.h"
#include "SkyBoxRenderer.h"
#include "SilhouetteRenderer.h"
#include "LightingRenderer.h"
#include "PostEffectRenderer.h"
#include "PickRenderer.h"

class MainRenderer
{
public:
	MainRenderer(int width, int height);
	~MainRenderer();

	void Render(Scene* scene, RenderContext* context);

	void Resize(int width, int height);

	PickInfo Pick(Scene* scene, RenderContext* context, unsigned int x, unsigned int y);

public:
	bool GetEnableSkybox();
	void SetEnableSkybox(bool value);

	bool GetEnableNormal();
	void SetEnableNormal(bool value);

	bool GetEnableLighting();
	void SetEnableLighting(bool value);

	bool GetEnablePostEffect();
	void SetEnablePostEffect(bool value);

private:
	void renderSkybox();

	void renderNormal(Scene* scene, RenderContext* context);

	void renderSihouette();

	void renderLighting();

protected:
	int m_Width;
	int m_Height;

	bool m_EnableSkybox;
	bool m_EnableNormal;
	bool m_EnableLighting;
	bool m_EnablePostEffect;

	PostEffectRenderer* m_PostRenderer;
	LightingRenderer* m_LightRenderer;
	SkyBoxRenderer* m_SkyboxRenderer;
	SilhouetteRenderer m_SilhouetteRenderer;
	PickRenderer* m_PickRenderer;
};

