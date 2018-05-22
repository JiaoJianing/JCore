#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: m_Terrain(0)
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

std::vector<ParticleSystem*>& Scene::GetParticleSystems()
{
	return m_ParticleSystems;
}

Terrain* Scene::GetTerrain()
{
	return m_Terrain;
}

void Scene::SetTerrain(Terrain* value)
{
	m_Terrain = value;
}
