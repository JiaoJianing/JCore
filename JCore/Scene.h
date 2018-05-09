#pragma once
#include <vector>
#include "Model.h"
#include "Cube.h"
#include "Camera.h"
#include "BaseLight.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Model*>& GetModels();

	std::vector<Cube*>& GetCubes();

	std::vector<Camera*>& GetCameras();

	std::vector<BaseLight*>& GetLights();

private:
	std::vector<Model*> m_Models;
	std::vector<Cube*> m_Cubes;
	std::vector<Camera*> m_Cameras;
	std::vector<BaseLight*> m_Lights;
};

