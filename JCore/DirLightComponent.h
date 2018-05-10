#pragma once
#include "BaseLightComponent.h"
#include "DirLight.h"

class DirLightComponent : public BaseLightComponent
{
public:
	DirLightComponent();
	~DirLightComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	virtual BaseLight* GetLight();

private:
	DirLight m_DirLight;
};

