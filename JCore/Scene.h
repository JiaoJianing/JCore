#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "BaseLight.h"
#include "CustomPrimitive.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Model*>& GetModels();

	std::vector<CustomPrimitive*>& GetCustomPrimitives();

	std::vector<Camera*>& GetCameras();

	std::vector<BaseLight*>& GetLights();

private:
	std::vector<Model*> m_Models;
	std::vector<CustomPrimitive*> m_CustomPrimitives;
	std::vector<Camera*> m_Cameras;
	std::vector<BaseLight*> m_Lights;
};

