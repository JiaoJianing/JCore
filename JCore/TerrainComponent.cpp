#include "stdafx.h"
#include "TerrainComponent.h"
#include "World.h"

TerrainComponent::TerrainComponent(const std::string& heightFile, unsigned char bitsPerPixel, unsigned int width, unsigned int height, 
	float heightScale /* = 30.0f */, float blockScale /* = 2.0f */)
	: m_Terrain(0)
{
	m_Terrain = new Terrain(heightScale, blockScale);
	m_Terrain->LoadHeightmap(heightFile, bitsPerPixel, width, height);
}


TerrainComponent::~TerrainComponent()
{
	if (m_Terrain != 0) {
		delete m_Terrain;
		m_Terrain = 0;
	}
}

stringT TerrainComponent::GetTypeName()
{
	return _T("TerrainComponent");
}

void TerrainComponent::Update(double curFrame, double deltaFrame)
{
}

void TerrainComponent::OnAddToWorld(World* world)
{
	world->GetScene()->SetTerrain(m_Terrain);
}

void TerrainComponent::OnRemoveFromWorld(World* world)
{
	world->GetScene()->SetTerrain(0);
}
