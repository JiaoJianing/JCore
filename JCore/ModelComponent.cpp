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

std::string ModelComponent::GetTypeName()
{
	return "ModelComponent";
}

void ModelComponent::Update(double curFrame, double deltaFrame)
{

}

void ModelComponent::Render()
{
	if (GetOwner()) {
		m_Shader.use();
		m_Shader.setMatrix4("model", GetOwner()->GetWorldTransform());

		m_Model.Draw(m_Shader);
	}
}
