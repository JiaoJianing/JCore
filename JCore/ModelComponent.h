#pragma once
#include "BaseComponent.h"
#include "Model.h"
#include "Shader.h"

class ModelComponent : public BaseComponent
{
public:
	ModelComponent(const std::string& path);
	~ModelComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	Model m_Model;
};

