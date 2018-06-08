#pragma once
#include "BaseRenderer.h"

class Water
{
public:
	Water();
	~Water();

	void Initialize(int width, int height);
	void Resize(int width, int height);

	void BeginRenderReflection();
	void EndRenderReflection();

	void BeginRenderRefraction();
	void EndRenderRefraction();

	void RenderWater(RenderContext* context, glm::vec3 lightPos=glm::vec3(100.0f, 150.0f, 100.0f));

	float& GetWaterHeight();

	std::vector<unsigned int>& GetCaustTextures();

	unsigned int GetReflectTexture();

private:
	unsigned int m_NormalTexture;
	unsigned int m_DudvTexture;
	unsigned int m_DepthTexture;
	unsigned int m_ReflectTexture;
	unsigned int m_RefractTexture;
	std::vector<unsigned int> m_CaustTextures;

	unsigned int m_ReflectFBO;
	unsigned int m_ReflectDepRBO;
	unsigned int m_RefractFBO;

	int m_ScreenWidth, m_ScreenHeight;

	float m_WaterHeight;

	unsigned int m_WaterVAO, m_WaterVBO;
	std::vector<glm::vec3> m_WaterPositions;
	std::vector<glm::vec2> m_WaterRefrCoords;
	std::vector<glm::vec2> m_WaterNormCoords;
	int m_WaterPositionSize, m_WaterRefrCoordSize, m_WaterNormCoordSize;
	float m_NormalMapScale;
	float m_WaterUV;
	float m_WaterFlow;
};

