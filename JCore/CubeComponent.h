#pragma once
#include "BaseComponent.h"
#include "CubePrimitive.h"

class CubeComponent : public BaseComponent
{
public:
	CubeComponent();
	CubeComponent(const std::string& diffuse, const std::string& normal, const std::string& specular);
	~CubeComponent();

	virtual void Update(double curFrame, double deltaFrame);

	virtual stringT GetTypeName();

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	CubePrimitive m_Cube;
};

