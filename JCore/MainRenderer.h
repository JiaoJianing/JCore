#pragma once
#include "BaseRenderer.h"
#include "PostEffectRenderer.h"
#include "PickRenderer.h"
#include "CubePrimitive.h"
#include "SpherePrimitive.h"
#include "ShadowMapRenderer.h"
#include "CSMRenderer.h"
#include "Water.h"

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
	bool& GetEnableSkybox();
	void SetEnableSkybox(bool value);

	bool& GetEnableNormal();
	void SetEnableNormal(bool value);

	bool& GetEnableLighting();
	void SetEnableLighting(bool value);

	bool GetEnablePostEffect();
	void SetEnablePostEffect(bool value);

	glm::vec3& GetSunDirection();
	void SetSunDirection(const glm::vec3& value);

private:
	void renderSkybox(Scene* scene, RenderContext* context);

	void renderNormal(Scene* scene, RenderContext* context);

	void renderSihouette(Scene* scene, RenderContext* context);

	//void renderLighting(Scene* scene, RenderContext* context);

	void renderLightingCSM(Scene* scene, RenderContext* context);

	void renderBillboard(Scene* scene, RenderContext* context);

	void renderParticleSys(Scene* scene, RenderContext* context);

	void renderModel(Scene* scene, RenderContext* context, Shader shader);

	void renderAnimationModel(Scene* scene, RenderContext* context, Shader shader);

	void renderCustomPrimitive(Scene* scene, RenderContext* context, Shader shader);

	void renderTerrain(Scene* scene, RenderContext* context);

	void prepareRenderWater(Scene* scene, RenderContext* context);

	void renderWater(Scene* scene, RenderContext* context);

	void renderWaterCSM(Scene* scene, RenderContext* context);

	//画出光源位置。用来调试
	void renderLightDebug(Scene* scene, RenderContext* context);

	void renderShadowDebug(Scene* scene, RenderContext* context);

	void prepareLightingParams(Scene* scene, Shader shader);

protected:	
	bool m_EnableSkybox;
	bool m_EnableNormal;
	bool m_EnableLighting;
	bool m_EnablePostEffect;

	PostEffectRenderer m_PostRenderer;
	PickRenderer m_PickRenderer;
	ShadowMapRenderer m_ShadowMapRenderer;
	CSMRenderer m_CSMRenderer;

	CubePrimitive m_CubeDebug;
	SpherePrimitive m_Sphere; //skydom
	//CubePrimitive m_Sphere; //skybox

	int m_Width;
	int m_Height;

	Quad m_ShadowDebugQuad;
	Water m_Water;
};

