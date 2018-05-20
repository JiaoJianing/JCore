#pragma once
#include "BaseRenderer.h"
#include "PostEffectRenderer.h"
#include "PickRenderer.h"
#include "CubePrimitive.h"
#include "SpherePrimitive.h"
#include "ShadowMapRenderer.h"

class MainRenderer
{
public:
	MainRenderer();
	~MainRenderer();

	void Render(Scene* scene, RenderContext* context);

	void Resize(int width, int height);

	void Initialize(int width, int height);

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
	void renderSkybox(Scene* scene, RenderContext* context);

	void renderNormal(Scene* scene, RenderContext* context);

	void renderSihouette(Scene* scene, RenderContext* context);

	void renderLighting(Scene* scene, RenderContext* context);

	void renderBillboard(Scene* scene, RenderContext* context);

	void renderParticleSys(Scene* scene, RenderContext* context);

	void renderModel(Scene* scene, RenderContext* context, Shader shader);

	void renderCustomPrimitive(Scene* scene, RenderContext* context, Shader shader);

	//画出光源位置。用来调试
	void renderLightDebug(Scene* scene, RenderContext* context);

protected:	
	bool m_EnableSkybox;
	bool m_EnableNormal;
	bool m_EnableLighting;
	bool m_EnablePostEffect;

	PostEffectRenderer m_PostRenderer;
	PickRenderer m_PickRenderer;
	ShadowMapRenderer m_ShadowMapRenderer;

	CubePrimitive m_CubeDebug;
	SpherePrimitive m_Sphere; //skydom
	//CubePrimitive m_Sphere; //skybox

	int m_Width;
	int m_Height;
};

