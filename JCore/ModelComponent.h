#pragma once
#include "BaseComponent.h"
#include "Model.h"
#include "Shader.h"

class ModelComponent : public BaseComponent
{
public:
	ModelComponent(const std::string& path);
	~ModelComponent();

	virtual std::string GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void Render();

private:
	Model m_Model;
	Shader m_Shader;
};

