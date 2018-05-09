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

std::vector<Cube*>& Scene::GetCubes()
{
	return m_Cubes;
}

std::vector<Sphere*>& Scene::GetSpheres()
{
	return m_Spheres;
}

std::vector<Camera*>& Scene::GetCameras()
{
	return m_Cameras;
}

std::vector<BaseLight*>& Scene::GetLights()
{
	return m_Lights;
}
