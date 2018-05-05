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

	virtual void Render();

	virtual void Render(Shader shader);

private:
	Model m_Model;
	Shader m_Shader;
};

