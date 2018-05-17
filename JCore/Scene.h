#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "BaseLight.h"
#include "CustomPrimitive.h"
#include "Billboard.h"
#include "ParticleSystem.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Model*>& GetModels();

	std::vector<CustomPrimitive*>& GetCustomPrimitives();

	std::vector<Camera*>& GetCameras();

	std::vector<BaseLight*>& GetLights();

	std::vector<Billboard*>& GetBillboards();

	std::vector<ParticleSystem*>& GetParticleSystems();

private:
	std::vector<Model*> m_Models;
	std::vector<CustomPrimitive*> m_CustomPrimitives;
	std::vector<Camera*> m_Cameras;
	std::vector<BaseLight*> m_Lights;
	std::vector<Billboard*> m_Billboards;
	std::vector<ParticleSystem*> m_ParticleSystems;
};

