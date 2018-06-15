#include "stdafx.h"
#include "CustomPrimitive.h"
#include "ResourceManager.h"

CustomPrimitive::CustomPrimitive()
	: m_Color(1.0f)
	, m_VBO(-1)
	, m_VAO(-1)
	, m_EBO(-1)
	, m_HighLight(false)
	, m_HighLightColor(1.0f)
{
}

CustomPrimitive::~CustomPrimitive()
{
}

void CustomPrimitive::Render(Shader shader)
{

}

void CustomPrimitive::RenderSimple(Shader shader)
{

}

void CustomPrimitive::RenderBoundingBox(Shader shader)
{
}

void CustomPrimitive::SetDiffuseTexture(const std::string& path)
{
	m_DiffuseTexture = *ResourceManager::getInstance()->LoadTexture(path, path);
}

void CustomPrimitive::SetNormalTexture(const std::string& path)
{
	m_NormalTexture = *ResourceManager::getInstance()->LoadTexture(path, path);
}

void CustomPrimitive::SetSpecularTexture(const std::string& path)
{
	m_SpecularTexture = *ResourceManager::getInstance()->LoadTexture(path, path);
}

void CustomPrimitive::SetColor(const glm::vec3& color)
{
	if (m_Color != color) {
		m_Color = color;
	}
}

glm::vec3& CustomPrimitive::GetColor()
{
	return m_Color;
}

void CustomPrimitive::SetWorldTransform(const glm::mat4& mat)
{
	if (m_WorldTransform != mat) {
		m_WorldTransform = mat;
	}
}

glm::mat4& CustomPrimitive::GetWorldTransform()
{
	return m_WorldTransform;
}

void CustomPrimitive::SetHighLight(bool value)
{
	m_HighLight = value;
}

bool CustomPrimitive::GetHighLight()
{
	return m_HighLight;
}

void CustomPrimitive::SetHighLightColor(const glm::vec3& color)
{
	if (m_HighLightColor != color) {
		m_HighLightColor = color;
	}
}

glm::vec3& CustomPrimitive::GetHighLightColor()
{
	return m_HighLightColor;
}

void CustomPrimitive::SetID(unsigned long id)
{
	m_ID = id;
}

unsigned long CustomPrimitive::GetID()
{
	return m_ID;
}

AABB& CustomPrimitive::GetAABB()
{
	return m_BoundingBox.GetAABB();
}
