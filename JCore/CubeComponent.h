#pragma once
#include "BaseComponent.h"
#include "Cube.h"

class CubeComponent : public BaseComponent
{
public:
	CubeComponent();
	CubeComponent(const std::string& texturePath);
	~CubeComponent();

	virtual void Update(double curFrame, double deltaFrame);

	virtual stringT GetTypeName();

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	Cube m_Cube;
};

