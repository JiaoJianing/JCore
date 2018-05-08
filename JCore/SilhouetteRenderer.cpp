#include "stdafx.h"
#include "SilhouetteRenderer.h"
#include "ResourceManager.h"
#include "Renderer.h"

SilhouetteRenderer::SilhouetteRenderer()
{
}


SilhouetteRenderer::~SilhouetteRenderer()
{
}

void SilhouetteRenderer::Render(Scene* scene, RenderContext* context)
{
	Shader shaderOutline = ResourceManager::getInstance()->GetShader("outline");
	shaderOutline.use().setMatrix4("view", context->MatView);
	shaderOutline.use().setMatrix4("projection", context->MatProj);

	//���Ƴ���
	//��ȾModel
	for (std::vector<Model*>::iterator it = scene->GetModels().begin(); it != scene->GetModels().end(); it++) {
		if ((*it)->GetHighLight()) {
			BeginRender();
			(*it)->Render(shaderOutline);
			EndRender();
		}
	}

	//��ȾCube
	for (std::vector<Cube*>::iterator it = scene->GetCubes().begin(); it != scene->GetCubes().end(); it++) {
		if ((*it)->GetHighLight()) {
			BeginRender();
			(*it)->Render(shaderOutline);
			EndRender();
		}
	}

	//��ȾSphere
	for (std::vector<Sphere*>::iterator it = scene->GetSpheres().begin(); it != scene->GetSpheres().end(); it++) {
		if ((*it)->GetHighLight()) {
			BeginRender();
			(*it)->Render(shaderOutline);
			EndRender();
		}
	}
}

void SilhouetteRenderer::BeginRender()
{
	//�л����߿�ģʽ�����ƴ�һЩ���ߣ����޳�����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glLineWidth(2.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SilhouetteRenderer::EndRender()
{
	//����״̬
	glDisable(GL_CULL_FACE);
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
