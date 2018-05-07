#pragma once
#include "typedefs.h"
#include "Shader.h"

class Node;
class BaseComponent
{
public:
	BaseComponent();
	virtual ~BaseComponent();

public:
	Node * GetOwner();
	void SetOwner(Node* value);

	virtual stringT GetTypeName() = 0;

	virtual void Update(double curFrame, double deltaFrame) = 0;

	virtual void Render() = 0;

	virtual void Render(Shader shader) = 0;

public:
	bool GetHighLight();
	void SetHighLight(bool value);

	bool GetPickable();
	void SetPickable(bool value);

	const glm::vec3& GetHighLightColor() const;
	void SetHighLightColor(const glm::vec3& color);

protected:
	Node * m_Owner;

	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	bool m_Pickable;
};

