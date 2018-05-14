#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

std::vector<Model*>& Scene::GetModels()
{
	return m_Models;
}

std::vector<CustomPrimitive*>& Scene::GetCustomPrimitives()
{
	return m_CustomPrimitives;
}

std::vector<Camera*>& Scene::GetCameras()
{
	return m_Cameras;
}

std::vector<BaseLight*>& Scene::GetLights()
{
	return m_Lights;
}

std::vector<Billboard*>& Scene::GetBillboards()
{
	return m_Billboards;
}
