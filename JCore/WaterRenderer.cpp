#include "stdafx.h"
#include "WaterRenderer.h"
#include "ResourceManager.h"

WaterRenderer::WaterRenderer()
	: m_NormalTexture(-1)
	, m_DudvTexture(-1)
	, m_DepthTexture(-1)
	, m_ReflectTexture(-1)
	, m_RefractTexture(-1)
	, m_ReflectFBO(-1)
	, m_ReflectDepRBO(-1)
	, m_RefractFBO(-1)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_WaterHeight(10.0f)
	, m_WaterVAO(-1)
	, m_WaterVBO(-1)
	, m_WaterPositionSize(0)
	, m_WaterRefrCoordSize(0)
	, m_WaterNormCoordSize(0)
	, m_NormalMapScale(0.25f)
	, m_WaterUV(35.0f)
	, m_WaterFlow(0.0015f)
	, m_WaterSize(256.0f)
{
}


WaterRenderer::~WaterRenderer()
{
	glDeleteFramebuffers(1, &m_ReflectFBO);
	glDeleteFramebuffers(1, &m_RefractFBO);
	glDeleteTextures(1, &m_ReflectTexture);
	glDeleteTextures(1, &m_RefractTexture);
	glDeleteTextures(1, &m_DepthTexture);
	glDeleteRenderbuffers(1, &m_ReflectDepRBO);

	glDeleteVertexArrays(1, &m_WaterVAO);
	glDeleteBuffers(1, &m_WaterVBO);
}

void WaterRenderer::Initialize(int width, int height)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;

#pragma region prepare fbo
	//generate reflect fbo
	glGenFramebuffers(1, &m_ReflectFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ReflectFBO);
	glGenTextures(1, &m_ReflectTexture);
	glBindTexture(GL_TEXTURE_2D, m_ReflectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ReflectTexture, 0);

	glGenRenderbuffers(1, &m_ReflectDepRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ReflectDepRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_ReflectDepRBO);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB0 error, status: 0x%x\n", Status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//generate refract fbo
	glGenFramebuffers(1, &m_RefractFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_RefractFBO);
	glGenTextures(1, &m_RefractTexture);
	glBindTexture(GL_TEXTURE_2D, m_RefractTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RefractTexture, 0);

	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB0 error, status: 0x%x\n", Status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region prepare textures
	m_NormalTexture = ResourceManager::getInstance()->LoadTexture("water_normal", "asset/resources/water/normalmap.bmp")->GetID();
	m_DudvTexture = ResourceManager::getInstance()->LoadTexture("water_dudv", "asset/resources/water/dudvmap.bmp")->GetID();
	for (int i = 0; i < 32; i++) {
		std::string caustTextureName = "asset/resources/water/caust" + std::to_string(i / 10) + std::to_string(i % 10) + ".bmp";
		m_CaustTextures.push_back(ResourceManager::getInstance()->LoadTexture(caustTextureName, caustTextureName)->GetID());
	}
#pragma endregion

#pragma region prepare vao
	m_WaterRefrCoords.resize(6);
	m_WaterNormCoords.resize(6);

	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, -m_WaterSize)); //back left
	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, m_WaterSize));  //front left
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, m_WaterSize));   //front right
	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, -m_WaterSize)); //back left
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, m_WaterSize));   //front right
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, -m_WaterSize));  //back right

	m_WaterPositionSize = sizeof(glm::vec3) * m_WaterPositions.size();
	m_WaterRefrCoordSize = sizeof(glm::vec2) * m_WaterRefrCoords.size();
	m_WaterNormCoordSize = sizeof(glm::vec2) * m_WaterNormCoords.size();

	glGenVertexArrays(1, &m_WaterVAO);
	glGenBuffers(1, &m_WaterVBO);

	glBindVertexArray(m_WaterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_WaterVBO);
	glBufferData(GL_ARRAY_BUFFER, m_WaterPositionSize + m_WaterRefrCoordSize + m_WaterNormCoordSize, 0, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_WaterPositionSize, &m_WaterPositions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_WaterPositionSize, m_WaterRefrCoordSize, &m_WaterRefrCoords[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_WaterPositionSize + m_WaterRefrCoordSize, m_WaterNormCoordSize, &m_WaterNormCoords[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(m_WaterPositionSize));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(m_WaterPositionSize + m_WaterRefrCoordSize));
	glBindVertexArray(0);
#pragma endregion
}

void WaterRenderer::Resize(int width, int height)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;
}

void WaterRenderer::BeginRenderReflection()
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ReflectFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterRenderer::EndRenderReflection()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
}

void WaterRenderer::BeginRenderRefraction()
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RefractFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterRenderer::EndRenderRefraction()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
}

void WaterRenderer::RenderWater(RenderContext* context, glm::vec3 lightPos/* =glm::vec3(100.0f, 150.0f, 100.0f) */)
{
	static float move = 0.0f;
	float move2 = move * m_NormalMapScale;
	float refrUV = m_WaterUV;
	float normalUV = m_WaterUV * m_NormalMapScale;

	move += m_WaterFlow;

	m_WaterPositions.clear();
	m_WaterRefrCoords.clear();
	m_WaterNormCoords.clear();	
	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, -m_WaterSize)); //back left
	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, m_WaterSize));  //front left
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, m_WaterSize));   //front right
	m_WaterPositions.push_back(glm::vec3(-m_WaterSize, m_WaterHeight, -m_WaterSize)); //back left
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, m_WaterSize));   //front right
	m_WaterPositions.push_back(glm::vec3(m_WaterSize, m_WaterHeight, -m_WaterSize));  //back right

	m_WaterRefrCoords.push_back(glm::vec2(0.0f, refrUV - move));//back left
	m_WaterRefrCoords.push_back(glm::vec2(0.0f, 0.0f - move));  //front left
	m_WaterRefrCoords.push_back(glm::vec2(refrUV, 0.0f - move));  //front right
	m_WaterRefrCoords.push_back(glm::vec2(0.0f, refrUV - move));//back left
	m_WaterRefrCoords.push_back(glm::vec2(refrUV, 0.0f - move));  //front right
	m_WaterRefrCoords.push_back(glm::vec2(refrUV, refrUV - move));//back right

	m_WaterNormCoords.push_back(glm::vec2(0.0f, normalUV + move2));
	m_WaterNormCoords.push_back(glm::vec2(0.0f, 0.0f + move2));
	m_WaterNormCoords.push_back(glm::vec2(normalUV, 0.0f + move2));
	m_WaterNormCoords.push_back(glm::vec2(0.0f, normalUV + move2));
	m_WaterNormCoords.push_back(glm::vec2(normalUV, 0.0f + move2));
	m_WaterNormCoords.push_back(glm::vec2(normalUV, normalUV + move2));

	Shader shaderWater = ResourceManager::getInstance()->GetShader("water");
	glDisable(GL_CULL_FACE);
	shaderWater.use();
	shaderWater.setMatrix4("view", context->MatView);
	shaderWater.setMatrix4("projection", context->MatProj);
	shaderWater.setVec3("viewPos", context->ViewPos);
	shaderWater.setVec3("lightPos", lightPos);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ReflectTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_DudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_RefractTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	
	glBindVertexArray(m_WaterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_WaterVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_WaterPositionSize, &m_WaterPositions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_WaterPositionSize, m_WaterRefrCoordSize, &m_WaterRefrCoords[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_WaterPositionSize + m_WaterRefrCoordSize, m_WaterNormCoordSize, &m_WaterNormCoords[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
}

float& WaterRenderer::GetWaterHeight()
{
	return m_WaterHeight;
}

std::vector<unsigned int>& WaterRenderer::GetCaustTextures()
{
	return m_CaustTextures;
}

unsigned int WaterRenderer::GetReflectTexture()
{
	return m_ReflectTexture;
}
