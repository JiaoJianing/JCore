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

private:
	Node * m_Owner;
};

