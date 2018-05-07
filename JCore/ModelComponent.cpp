#include "stdafx.h"
#include "Node.h"
#include "ModelComponent.h"
#include "ResourceManager.h"

ModelComponent::ModelComponent(const std::string& path)
{
	m_Model.LoadModel(path.c_str());
	m_Shader = ResourceManager::getInstance()->GetShader("model");
}

ModelComponent::~ModelComponent()
{
}

stringT ModelComponent::GetTypeName()
{
	return _T("ModelComponent");
}

void ModelComponent::Update(double curFrame, double deltaFrame)
{

}

void ModelComponent::Render()
{
	if (GetOwner()) {

		if (GetHighLight()) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glLineWidth(2.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			ResourceManager::getInstance()->GetShader("outline").use().setMatrix4("model", GetOwner()->GetWorldTransform());
			m_Model.Draw(ResourceManager::getInstance()->GetShader("outline"));
		}

		glDisable(GL_CULL_FACE);
		glLineWidth(1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_Shader.use();
		m_Shader.setMatrix4("model", GetOwner()->GetWorldTransform());
		m_Shader.setInt("highLight", GetHighLight());
		m_Shader.setVec3("highLightColor", GetHighLightColor());

		m_Model.Draw(m_Shader);
	}
}

void ModelComponent::Render(Shader shader)
{
	if (GetOwner()) {
		shader.use();
		shader.setMatrix4("model", GetOwner()->GetWorldTransform());

		m_Model.Draw(shader);
	}
}
