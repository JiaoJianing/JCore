#include "stdafx.h"
#include "LightingRenderer.h"
#include "ResourceManager.h"
#include "DirLight.h"
#include "PointLight.h"
#include "SpotLight.h"

LightingRenderer::LightingRenderer(int width, int height)
	:Renderer(width, height)
{
	m_CubeDebug.SetColor(glm::vec3(1.0f));
}


LightingRenderer::~LightingRenderer()
{
}

void LightingRenderer::Initialize()
{
}

void LightingRenderer::Render(Scene* scene, RenderContext* context)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//��Ⱦ����
	m_SilhouetteRenderer.Render(scene, context);

	//��ȾModel
	Shader shaderPhong = ResourceManager::getInstance()->GetShader("phong");
	shaderPhong.use();
	shaderPhong.setMatrix4("view", context->MatView);
	shaderPhong.setMatrix4("projection", context->MatProj);
	shaderPhong.setVec3("viewPos", context->ViewPos);

	//���ù��ղ���
	int pointlightNum = 0;
	int spotlightNum = 0;
	int dirlightNum = 0;
	for (std::vector<BaseLight*>::iterator it = scene->GetLights().begin(); it != scene->GetLights().end(); it++) {
		//�����
		if (dynamic_cast<DirLight*>(*it) != 0) {
			DirLight* light = dynamic_cast<DirLight*>(*it);
			shaderPhong.setVec3("dirLights[" + std::to_string(dirlightNum) + "].base.color", light->GetLightColor());
			shaderPhong.setFloat("dirLights[" + std::to_string(dirlightNum) + "].base.ambientIntensity", light->GetAmbientIntensity());
			shaderPhong.setFloat("dirLights[" + std::to_string(dirlightNum) + "].base.diffuseIntensity", light->GetDiffuseIntensity());
			shaderPhong.setVec3("dirLights[" + std::to_string(dirlightNum) + "].direction", light->GetLightPos());
			dirlightNum++;
		}
		//�۹��
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
		//���Դ
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

	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderPhong);
	}

	//��ȾCube
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		(*it)->Render(shaderPhong);
	}

	//���������Դλ��
	if (true) {
		RenderLightDebug(scene, context);
	}
}

void LightingRenderer::Resize(int width, int height)
{

}

void LightingRenderer::RenderLightDebug(Scene* scene, RenderContext* context)
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
		m_CubeDebug.RenderDebug(shaderLightDbg);
	}
}
