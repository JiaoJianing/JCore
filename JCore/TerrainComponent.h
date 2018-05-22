#pragma once
#include "BaseComponent.h"
#include "Terrain.h"

class TerrainComponent : public BaseComponent
{
public:
	TerrainComponent(const std::string& heightFile, unsigned char bitsPerPixel, unsigned int width, unsigned int height,
		float heightScale = 30.0f, float blockScale = 2.0f);
	~TerrainComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	Terrain * m_Terrain;
};

