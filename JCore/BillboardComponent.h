#pragma once
#include "BaseComponent.h"
#include "Billboard.h"

class BillboardComponent : public BaseComponent
{
public:
	BillboardComponent(const std::string& texturePath);
	~BillboardComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	Billboard m_Billboard;
};

