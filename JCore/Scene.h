#pragma once
#include <vector>
#include "Model.h"
#include "Camera.h"
#include "BaseLight.h"
#include "CustomPrimitive.h"
#include "Billboard.h"

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

private:
	std::vector<Model*> m_Models;
	std::vector<CustomPrimitive*> m_CustomPrimitives;
	std::vector<Camera*> m_Cameras;
	std::vector<BaseLight*> m_Lights;
	std::vector<Billboard*> m_Billboards;
};

