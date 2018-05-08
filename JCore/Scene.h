#pragma once
#include <vector>
#include "Model.h"
#include "Cube.h"
#include "Sphere.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Model*>& GetModels();

	std::vector<Cube*>& GetCubes();
	
	std::vector<Sphere*>& GetSpheres();

private:
	std::vector<Model*> m_Models;
	std::vector<Cube*> m_Cubes;
	std::vector<Sphere*> m_Spheres;
};

