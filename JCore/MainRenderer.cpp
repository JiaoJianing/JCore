#include "stdafx.h"
#include "MainRenderer.h"
#include "Model.h"
#include "ResourceManager.h"
#include "CustomPrimitive.h"
#include "DirLight.h"
#include "PointLight.h"
#include "SpotLight.h"

MainRenderer::MainRenderer()
	: m_EnableLighting(false)
	, m_EnableSkybox(false)
	, m_EnableNormal(false)
	, m_EnablePostEffect(false)
{
	m_CubeDebug.SetColor(glm::vec3(1.0f));
}


MainRenderer::~MainRenderer()
{
}

void MainRenderer::Render(Scene* scene, RenderContext* context)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);

	if (m_EnableLighting) {
		//m_ShadowMapRenderer.Render(scene, context);
		m_CSMRenderer.Render(scene, context);
	}

	if (false) {
		renderShadowDebug(scene, context);
		return;
	}

	glViewport(0, 0, m_Width, m_Height);

	//后期开始
	if (m_EnablePostEffect) {
		m_PostRenderer.BeginRender();
	}

	//渲染天空盒
	if (m_EnableSkybox) {
		renderSkybox(scene, context);
	}

	//是否使用光照
	if (m_EnableLighting) {
		//renderLighting(scene, context);
		renderLightingCSM(scene, context);
	}
	else {
		//渲染地形
		renderTerrain(scene, context);

		Shader shaderModel = ResourceManager::getInstance()->GetShader("model");
		Shader shaderModel_Animation = ResourceManager::getInstance()->GetShader("model_animation");
		shaderModel.use();
		shaderModel.setMatrix4("view", context->MatView);
		shaderModel.setMatrix4("projection", context->MatProj);
		shaderModel.setVec3("viewPos", context->ViewPos);

		//渲染模型
		renderModel(scene, context, shaderModel);

		//渲染自定义图元
		renderCustomPrimitive(scene, context, shaderModel);

		shaderModel_Animation.use();
		shaderModel_Animation.setMatrix4("view", context->MatView);
		shaderModel_Animation.setMatrix4("projection", context->MatProj);
		shaderModel_Animation.setVec3("viewPos", context->ViewPos);
		renderAnimationModel(scene, context, shaderModel_Animation);
	}

	//渲染标牌
	renderBillboard(scene, context);

	//渲染粒子系统
	renderParticleSys(scene, context);

	//输出法线
	if (m_EnableNormal) {
		renderNormal(scene, context);
	}

	//描边
	renderSihouette(scene, context);

	//后期结束
	if (m_EnablePostEffect) {
		m_PostRenderer.EndRender();
		m_PostRenderer.Render(scene, context);
	}
}

void MainRenderer::Resize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_PostRenderer.Resize(width, height);
	m_PickRenderer.Resize(width, height);
	m_ShadowMapRenderer.Resize(width, height);
	m_CSMRenderer.Resize(width, height);
}

void MainRenderer::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_PostRenderer.Initialize(width, height);
	m_PickRenderer.Initialize(width, height);
	m_ShadowMapRenderer.Initialize(width, height);
	m_CSMRenderer.Initialize(width, height);
}

PickInfo MainRenderer::Pick(Scene* scene, RenderContext* context, unsigned int x, unsigned int y)
{
	m_PickRenderer.Render(scene, context);

	return m_PickRenderer.Pick(x, y);
}

bool& MainRenderer::GetEnableSkybox()
{
	return m_EnableSkybox;
}

void MainRenderer::SetEnableSkybox(bool value)
{
	m_EnableSkybox = value;
}

bool& MainRenderer::GetEnableNormal()
{
	return m_EnableNormal;
}

void MainRenderer::SetEnableNormal(bool value)
{
	m_EnableNormal = value;
}

bool& MainRenderer::GetEnableLighting()
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

glm::vec3& MainRenderer::GetSunDirection()
{
	return m_CSMRenderer.GetSunDirection();
}

void MainRenderer::SetSunDirection(const glm::vec3& value)
{
	m_CSMRenderer.SetSunDirection(value);
}

void MainRenderer::renderSkybox(Scene* scene, RenderContext* context)
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	//glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	Shader shaderSkybox = ResourceManager::getInstance()->GetShader("skybox");
	Texture* textureSkybox = ResourceManager::getInstance()->GetTexture("skybox3");
	shaderSkybox.use();
	shaderSkybox.setMatrix4("view", context->MatView);
	shaderSkybox.setMatrix4("projection", context->MatProj);
	glm::mat4 model;
	model = glm::translate(model, context->ViewPos);
	shaderSkybox.setMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox->GetID());

	m_Sphere.RenderSimple(shaderSkybox);

	//glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}

void MainRenderer::renderNormal(Scene* scene, RenderContext* context)
{
	Shader shaderShowNormal = ResourceManager::getInstance()->GetShader("show_normal");
	Shader shaderShowNormal_Animation = ResourceManager::getInstance()->GetShader("show_normal_animation");
	shaderShowNormal.use();
	shaderShowNormal.setMatrix4("view", context->MatView);
	shaderShowNormal.setMatrix4("projection", context->MatProj);

	//渲染模型
	renderModel(scene, context, shaderShowNormal);

	//渲染自定义图元
	renderCustomPrimitive(scene, context, shaderShowNormal);

	shaderShowNormal_Animation.use();
	shaderShowNormal_Animation.setMatrix4("view", context->MatView);
	shaderShowNormal_Animation.setMatrix4("projection", context->MatProj);
	renderAnimationModel(scene, context, shaderShowNormal_Animation);
}

void MainRenderer::renderSihouette(Scene* scene, RenderContext* context)
{
	Shader shaderOutline = ResourceManager::getInstance()->GetShader("outline");
	shaderOutline.use().setMatrix4("view", context->MatView);
	shaderOutline.use().setMatrix4("projection", context->MatProj);

	//查找需要描边的对象
	std::vector<Model*> highlightModels;
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		if ((*it)->GetHighLight()) {
			highlightModels.push_back(*it);
		}
	}

	std::vector<CustomPrimitive*> highlightPrimitives;
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		if ((*it)->GetHighLight()) {
			highlightPrimitives.push_back(*it);
		}
	}

	if (highlightModels.size() > 0 || highlightPrimitives.size() > 0) {
		//切换到线框模式，绘制粗一些的线，并剔除背面
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glLineWidth(2.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//渲染
		for (std::vector<Model*>::iterator it = highlightModels.begin(); it != highlightModels.end(); it++) {
			(*it)->Render(shaderOutline);
		}
		for (std::vector<CustomPrimitive*>::iterator it = highlightPrimitives.begin(); it != highlightPrimitives.end(); it++) {
			(*it)->Render(shaderOutline);
		}

		//重置状态
		glDisable(GL_CULL_FACE);
		glLineWidth(1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	std::vector<Model*> highlightAnimationModels;
	for (std::vector<Model*>::iterator it = scene->GetAnimationModels().begin(); it != scene->GetAnimationModels().end(); it++) {
		if ((*it)->GetHighLight()) {
			highlightAnimationModels.push_back(*it);
		}
	}

	Shader shaderOutline_Animation = ResourceManager::getInstance()->GetShader("outline_animation");
	shaderOutline_Animation.use().setMatrix4("view", context->MatView);
	shaderOutline_Animation.use().setMatrix4("projection", context->MatProj);
	if (highlightAnimationModels.size() > 0) {
		//切换到线框模式，绘制粗一些的线，并剔除背面
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glLineWidth(2.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//渲染
		for (std::vector<Model*>::iterator it = highlightAnimationModels.begin(); it != highlightAnimationModels.end(); it++) {
			(*it)->Render(shaderOutline_Animation);
		}

		//重置状态
		glDisable(GL_CULL_FACE);
		glLineWidth(1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void MainRenderer::renderLighting(Scene* scene, RenderContext* context)
{
	Shader shaderPhong = ResourceManager::getInstance()->GetShader("phong");
	shaderPhong.use();
	shaderPhong.setMatrix4("view", context->MatView);
	shaderPhong.setMatrix4("projection", context->MatProj);
	shaderPhong.setVec3("viewPos", context->ViewPos);
	shaderPhong.setMatrix4("lightSpaceMat", m_ShadowMapRenderer.GetLightSpaceMat());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMapTexture());

	//设置光照参数
	int pointlightNum = 0;
	int spotlightNum = 0;
	int dirlightNum = 0;
	for (std::vector<BaseLight*>::iterator it = scene->GetLights().begin(); it != scene->GetLights().end(); it++) {
		//方向光
		if (dynamic_cast<DirLight*>(*it) != 0) {
			DirLight* light = dynamic_cast<DirLight*>(*it);
			shaderPhong.setVec3("dirLights[" + std::to_string(dirlightNum) + "].base.color", light->GetLightColor());
			shaderPhong.setFloat("dirLights[" + std::to_string(dirlightNum) + "].base.ambientIntensity", light->GetAmbientIntensity());
			shaderPhong.setFloat("dirLights[" + std::to_string(dirlightNum) + "].base.diffuseIntensity", light->GetDiffuseIntensity());
			shaderPhong.setVec3("dirLights[" + std::to_string(dirlightNum) + "].direction", light->GetLightPos());
			dirlightNum++;
		}
		//聚光灯
		else if (dynamic_cast<SpotLight*>(*it) != 0) {
			SpotLight* light = dynamic_cast<SpotLight*>(*it);
			shaderPhong.setVec3("spotLights[" + std::to_string(spotlightNum) + "].base.base.color", light->GetLightColor());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].base.base.ambientIntensity", light->GetAmbientIntensity());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].base.base.diffuseIntensity", light->GetDiffuseIntensity());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].base.attenuation.constant", light->GetConstant());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].base.attenuation.linear", light->GetLinear());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].base.attenuation.exp", light->GetExp());
			shaderPhong.setVec3("spotLights[" + std::to_string(spotlightNum) + "].base.position", light->GetLightPos());
			shaderPhong.setVec3("spotLights[" + std::to_string(spotlightNum) + "].direction", light->GetDirection());
			shaderPhong.setFloat("spotLights[" + std::to_string(spotlightNum) + "].cutoff", glm::cos(glm::radians(light->GetCutOff())));
			spotlightNum++;
		}
		//点光源
		else if (dynamic_cast<PointLight*>(*it) != 0) {
			PointLight* light = dynamic_cast<PointLight*>(*it);
			shaderPhong.setVec3("pointLights[" + std::to_string(pointlightNum) + "].base.color", light->GetLightColor());
			shaderPhong.setFloat("pointLights[" + std::to_string(pointlightNum) + "].base.ambientIntensity", light->GetAmbientIntensity());
			shaderPhong.setFloat("pointLights[" + std::to_string(pointlightNum) + "].base.diffuseIntensity", light->GetDiffuseIntensity());
			shaderPhong.setFloat("pointLights[" + std::to_string(pointlightNum) + "].attenuation.constant", light->GetConstant());
			shaderPhong.setFloat("pointLights[" + std::to_string(pointlightNum) + "].attenuation.linear", light->GetLinear());
			shaderPhong.setFloat("pointLights[" + std::to_string(pointlightNum) + "].attenuation.exp", light->GetExp());
			shaderPhong.setVec3("pointLights[" + std::to_string(pointlightNum) + "].position", light->GetLightPos());
			pointlightNum++;
		}
	}
	shaderPhong.setInt("pointLightNum", pointlightNum);
	shaderPhong.setInt("spotLightNum", spotlightNum);
	shaderPhong.setInt("dirLightNum", dirlightNum);
	
	//渲染模型
	renderModel(scene, context, shaderPhong);

	//渲染自定义图元
	renderCustomPrimitive(scene, context, shaderPhong);

	//调试输出光源位置
	if (false) {
		renderLightDebug(scene, context);
	}
}

void MainRenderer::renderLightingCSM(Scene* scene, RenderContext* context)
{
	Shader shaderCSM_Terrain = ResourceManager::getInstance()->GetShader("csm_terrain");
	shaderCSM_Terrain.use();
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(0)->ShadowTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(1)->ShadowTexture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(2)->ShadowTexture);
	shaderCSM_Terrain.setMatrix4("view", context->MatView);
	shaderCSM_Terrain.setMatrix4("projection", context->MatProj);
	shaderCSM_Terrain.setMatrix4("lightViewProj[0]", m_CSMRenderer.GetCascadeAt(0)->LightViewProj);
	shaderCSM_Terrain.setMatrix4("lightViewProj[1]", m_CSMRenderer.GetCascadeAt(1)->LightViewProj);
	shaderCSM_Terrain.setMatrix4("lightViewProj[2]", m_CSMRenderer.GetCascadeAt(2)->LightViewProj);
	shaderCSM_Terrain.setFloat("cascadeSpace[0]", m_CSMRenderer.GetCascadeAt(0)->CascadeSpace);
	shaderCSM_Terrain.setFloat("cascadeSpace[1]", m_CSMRenderer.GetCascadeAt(1)->CascadeSpace);
	shaderCSM_Terrain.setFloat("cascadeSpace[2]", m_CSMRenderer.GetCascadeAt(2)->CascadeSpace);
	shaderCSM_Terrain.setVec3("lightDirection", -m_CSMRenderer.GetSunDirection());
	scene->GetTerrain()->Render(shaderCSM_Terrain);

	Shader shaderCSM_Model = ResourceManager::getInstance()->GetShader("csm_model");
	shaderCSM_Model.use();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(0)->ShadowTexture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(1)->ShadowTexture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(2)->ShadowTexture);
	shaderCSM_Model.setMatrix4("view", context->MatView);
	shaderCSM_Model.setMatrix4("projection", context->MatProj);
	shaderCSM_Model.setMatrix4("lightViewProj[0]", m_CSMRenderer.GetCascadeAt(0)->LightViewProj);
	shaderCSM_Model.setMatrix4("lightViewProj[1]", m_CSMRenderer.GetCascadeAt(1)->LightViewProj);
	shaderCSM_Model.setMatrix4("lightViewProj[2]", m_CSMRenderer.GetCascadeAt(2)->LightViewProj);
	shaderCSM_Model.setFloat("cascadeSpace[0]", m_CSMRenderer.GetCascadeAt(0)->CascadeSpace);
	shaderCSM_Model.setFloat("cascadeSpace[1]", m_CSMRenderer.GetCascadeAt(1)->CascadeSpace);
	shaderCSM_Model.setFloat("cascadeSpace[2]", m_CSMRenderer.GetCascadeAt(2)->CascadeSpace);
	shaderCSM_Model.setVec3("dirLight.base.color", glm::vec3(1.0f));
	shaderCSM_Model.setFloat("dirLight.base.ambientIntensity", 0.05f);
	shaderCSM_Model.setFloat("dirLight.base.diffuseIntensity", 0.8f);
	shaderCSM_Model.setVec3("dirLight.direction", -m_CSMRenderer.GetSunDirection());

	//渲染模型
	renderModel(scene, context, shaderCSM_Model);

	//渲染自定义图元
	renderCustomPrimitive(scene, context, shaderCSM_Model);

	Shader shaderCSM_Model_Animation = ResourceManager::getInstance()->GetShader("csm_model_animation");
	shaderCSM_Model_Animation.use();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(0)->ShadowTexture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(1)->ShadowTexture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_CSMRenderer.GetCascadeAt(2)->ShadowTexture);
	shaderCSM_Model_Animation.setMatrix4("view", context->MatView);
	shaderCSM_Model_Animation.setMatrix4("projection", context->MatProj);
	shaderCSM_Model_Animation.setMatrix4("lightViewProj[0]", m_CSMRenderer.GetCascadeAt(0)->LightViewProj);
	shaderCSM_Model_Animation.setMatrix4("lightViewProj[1]", m_CSMRenderer.GetCascadeAt(1)->LightViewProj);
	shaderCSM_Model_Animation.setMatrix4("lightViewProj[2]", m_CSMRenderer.GetCascadeAt(2)->LightViewProj);
	shaderCSM_Model_Animation.setFloat("cascadeSpace[0]", m_CSMRenderer.GetCascadeAt(0)->CascadeSpace);
	shaderCSM_Model_Animation.setFloat("cascadeSpace[1]", m_CSMRenderer.GetCascadeAt(1)->CascadeSpace);
	shaderCSM_Model_Animation.setFloat("cascadeSpace[2]", m_CSMRenderer.GetCascadeAt(2)->CascadeSpace);
	shaderCSM_Model_Animation.setVec3("dirLight.base.color", glm::vec3(1.0f));
	shaderCSM_Model_Animation.setFloat("dirLight.base.ambientIntensity", 0.05f);
	shaderCSM_Model_Animation.setFloat("dirLight.base.diffuseIntensity", 0.8f);
	shaderCSM_Model_Animation.setVec3("dirLight.direction", -m_CSMRenderer.GetSunDirection());
	renderAnimationModel(scene, context, shaderCSM_Model_Animation);
}

void MainRenderer::renderBillboard(Scene* scene, RenderContext* context)
{
	Shader shaderBillboard = ResourceManager::getInstance()->GetShader("billboard");
	shaderBillboard.use();
	shaderBillboard.setMatrix4("view", context->MatView);
	shaderBillboard.setMatrix4("projection", context->MatProj);
	shaderBillboard.setVec3("viewPos", context->ViewPos);
	for (std::vector<Billboard*>::iterator it = scene->GetBillboards().begin(); it != scene->GetBillboards().end(); it++) {
		(*it)->Render(shaderBillboard);
	}
}

void MainRenderer::renderParticleSys(Scene* scene, RenderContext* context)
{
	Shader shaderParticleSys = ResourceManager::getInstance()->GetShader("particle_system");
	shaderParticleSys.use();
	shaderParticleSys.setMatrix4("view", context->MatView);
	shaderParticleSys.setMatrix4("projection", context->MatProj);
	shaderParticleSys.setVec3("viewPos", context->ViewPos);
	for (std::vector<ParticleSystem*>::iterator it = scene->GetParticleSystems().begin(); it != scene->GetParticleSystems().end(); it++) {
		(*it)->Render(shaderParticleSys);
	}
}

void MainRenderer::renderModel(Scene* scene, RenderContext* context, Shader shader)
{
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shader);
	}
}

void MainRenderer::renderAnimationModel(Scene* scene, RenderContext* context, Shader shader)
{
	for (std::vector<Model*>::iterator it = scene->GetAnimationModels().begin(); it != scene->GetAnimationModels().end(); it++) {
		(*it)->Render(shader);
	}
}

void MainRenderer::renderCustomPrimitive(Scene* scene, RenderContext* context, Shader shader)
{
	for (std::vector<CustomPrimitive*>::iterator it = scene->GetCustomPrimitives().begin(); it != scene->GetCustomPrimitives().end(); it++) {
		(*it)->Render(shader);
	}
}

void MainRenderer::renderTerrain(Scene* scene, RenderContext* context)
{
	Terrain* terrain = scene->GetTerrain();
	if (terrain != 0) {
		Shader shaderTerrain = ResourceManager::getInstance()->GetShader("terrain");
		shaderTerrain.use();
		shaderTerrain.setMatrix4("view", context->MatView);
		shaderTerrain.setMatrix4("projection", context->MatProj);
		terrain->Render(shaderTerrain);
	}
}

void MainRenderer::renderLightDebug(Scene* scene, RenderContext* context)
{
	Shader shaderLightDbg = ResourceManager::getInstance()->GetShader("light_debug");
	shaderLightDbg.use();
	shaderLightDbg.setMatrix4("view", context->MatView);
	shaderLightDbg.setMatrix4("projection", context->MatProj);
	for (std::vector<BaseLight*>::iterator it = scene->GetLights().begin(); it != scene->GetLights().end(); it++) {
		glm::mat4 model;
		model = glm::translate(model, (*it)->GetLightPos());
		model = glm::scale(model, glm::vec3(0.2f));
		shaderLightDbg.setMatrix4("model", model);
		m_CubeDebug.RenderSimple(shaderLightDbg);
	}
}

void MainRenderer::renderShadowDebug(Scene* scene, RenderContext* context)
{
	Shader shaderShadowDbg = ResourceManager::getInstance()->GetShader("shadow_debug");
	shaderShadowDbg.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMapTexture());

	m_ShadowDebugQuad.Render(shaderShadowDbg);
}

void RenderContext::GetParamsFromCamera(CameraComponent* camera)
{
	MatView = camera->GetViewTransform();
	MatProj = camera->GetProjectionTransform();
	ViewPos = camera->GetPos();
	ViewTarget = camera->GetTarget();
	ViewUp = camera->GetUp();
}

void RenderContext::UpdateTransform()
{

}
