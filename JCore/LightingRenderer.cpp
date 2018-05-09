#include "stdafx.h"
#include "LightingRenderer.h"
#include "ResourceManager.h"
#include "DirLight.h"

LightingRenderer::LightingRenderer(int width, int height)
	:Renderer(width, height)
{
}


LightingRenderer::~LightingRenderer()
{
}

void LightingRenderer::Initialize()
{
}

void LightingRenderer::Render(Scene* scene, RenderContext* context)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	for (std::vector<BaseLight*>::iterator it = scene->GetLights().begin(); it != scene->GetLights().end(); it++) {
		if (dynamic_cast<DirLight*>(*it)) {
			DirLight* light = dynamic_cast<DirLight*>(*it);
			shaderPhong.setVec3("dirLight.lightColor", light->GetLightColor());
			shaderPhong.setVec3("dirLight.lightPos", light->GetLightPos());
			shaderPhong.setFloat("dirLight.ambientIntensity", light->GetAmbientIntensity());
			shaderPhong.setFloat("dirLight.diffuseIntensity", light->GetDiffuseIntensity());
		}
	}

	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		(*it)->Render(shaderPhong);
	}

	//��ȾCube
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		(*it)->Render(shaderPhong);
	}

	//��ȾSphere
	for (std::vector<Sphere*>::iterator it = scene->GetSpheres().begin(); it != scene->GetSpheres().end(); it++) {
		(*it)->Render(shaderPhong);
	}
}

void LightingRenderer::Resize(int width, int height)
{

}
