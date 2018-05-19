#include "stdafx.h"
#include "MainRenderer.h"
#include "Model.h"
#include "ResourceManager.h"
#include "CustomPrimitive.h"

MainRenderer::MainRenderer(int width, int height)
	: m_Width(width)
	, m_Height(height)
	, m_PostRenderer(0)
	, m_LightRenderer(0)
	, m_SkyboxRenderer(0)
	, m_PickRenderer(0)
	, m_EnableLighting(false)
	, m_EnableSkybox(false)
	, m_EnableNormal(false)
	, m_EnablePostEffect(false)
{
	m_PostRenderer = new PostEffectRenderer(width, height);
	m_PostRenderer->Initialize();
	m_LightRenderer = new LightingRenderer(width, height);
	m_LightRenderer->Initialize();
	m_SkyboxRenderer = new SkyBoxRenderer(width, height);
	m_SkyboxRenderer->Initialize();
	m_PickRenderer = new PickRenderer(width, height);
	m_PickRenderer->Initialize();
}


MainRenderer::~MainRenderer()
{
	if (m_PostRenderer != 0) {
		delete m_PostRenderer;
		m_PostRenderer = 0;
	}

	if (m_LightRenderer != 0) {
		delete m_LightRenderer;
		m_LightRenderer = 0;
	}

	if (m_SkyboxRenderer != 0) {
		delete m_SkyboxRenderer;
		m_SkyboxRenderer = 0;
	}

	if (m_PickRenderer != 0) {
		delete m_PickRenderer;
		m_PickRenderer = 0;
	}
}

void MainRenderer::Render(Scene* scene, RenderContext* context)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);

	//后期开始
	if (GetEnablePostEffect()) {
		m_PostRenderer->BeginRender();
	}

	//渲染天空盒
	if (m_EnableSkybox) {
		if (m_SkyboxRenderer != 0) {
			m_SkyboxRenderer->Render(scene, context);
		}
	}

	//是否使用光照
	if (m_EnableLighting) {
		m_LightRenderer->Render(scene, context);
	}
	else {
		//渲染Model
		Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
		shaderModel.use();
		shaderModel.setMatrix4("view", context->MatView);
		shaderModel.setMatrix4("projection", context->MatProj);
		shaderModel.setVec3("viewPos", context->ViewPos);
		for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
			(*it)->Render(shaderModel);
		}

		//渲染自定义图元
		for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
			(*it)->Render(shaderModel);
		}

		//渲染Billboard
		Shader shaderBillboard = ResourceManager::getInstance()->GetShader("billboard");
		shaderBillboard.use();
		shaderBillboard.setMatrix4("view", context->MatView);
		shaderBillboard.setMatrix4("projection", context->MatProj);
		shaderBillboard.setVec3("viewPos", context->ViewPos);
		for (std::vector<Billboard*>::iterator it = scene->GetBillboards().begin(); it != scene->GetBillboards().end(); it++) {
			(*it)->Render(shaderBillboard);
		}

		//渲染粒子系统
		Shader shaderParticleSys = ResourceManager::getInstance()->GetShader("particle_system");
		shaderParticleSys.use();
		shaderParticleSys.setMatrix4("view", context->MatView);
		shaderParticleSys.setMatrix4("projection", context->MatProj);
		shaderParticleSys.setVec3("viewPos", context->ViewPos);
		for (std::vector<ParticleSystem*>::iterator it = scene->GetParticleSystems().begin(); it != scene->GetParticleSystems().end(); it++) {
			(*it)->Render(shaderParticleSys);
		}
	}

	//是否输出法线
	if (m_EnableNormal) {
		renderNormal(scene, context);
	}

	//轮廓渲染
	m_SilhouetteRenderer.Render(scene, context);

	//后期结束
	if (GetEnablePostEffect()) {
		m_PostRenderer->EndRender();
		m_PostRenderer->Render(scene, context);
	}
}

void MainRenderer::Resize(int width, int height)
{
	m_PostRenderer->Resize(width, height);
	m_LightRenderer->Resize(width, height);
	m_SkyboxRenderer->Resize(width, height);
	m_PickRenderer->Resize(width, height);
}

PickInfo MainRenderer::Pick(Scene* scene, RenderContext* context, unsigned int x, unsigned int y)
{
	m_PickRenderer->Render(scene, context);

	return m_PickRenderer->Pick(x, y);
}

bool MainRenderer::GetEnableSkybox()
{
	return m_EnableSkybox;
}

void MainRenderer::SetEnableSkybox(bool value)
{
	m_EnableSkybox = value;
}

bool MainRenderer::GetEnableNormal()
{
	return m_EnableNormal;
}

void MainRenderer::SetEnableNormal(bool value)
{
	m_EnableNormal = value;
}

bool MainRenderer::GetEnableLighting()
{
	return m_EnableLighting;
}

void MainRenderer::SetEnableLighting(bool value)
{
	m_EnableLighting = value;
}

bool MainRenderer::GetEnablePostEffect()
{
	return m_EnablePostEffect;
}

void MainRenderer::SetEnablePostEffect(bool value)
{
	m_EnablePostEffect = value;
}

void MainRenderer::renderSkybox()
{

}

void MainRenderer::renderNormal(Scene* scene, RenderContext* context)
{
	Shader shaderShowNormal = ResourceManager::getInstance()->GetShader("show_normal");
	shaderShowNormal.use();
	shaderShowNormal.setMatrix4("view", context->MatView);
	shaderShowNormal.setMatrix4("projection", context->MatProj);

	//渲染Model
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderShowNormal);
	}

	//渲染自定义图元
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shaderShowNormal);
	}
}

void MainRenderer::renderSihouette()
{

}

void MainRenderer::renderLighting()
{

}

void RenderContext::GetParamsFromCamera(CameraComponent* camera)
{
	MatView = camera->GetViewTransform();
	MatProj = camera->GetProjectionTransform();
	ViewPos = camera->GetPos();
}

void RenderContext::UpdateTransform()
{

}
