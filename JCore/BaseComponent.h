#pragma once
#include "typedefs.h"
#include "Shader.h"

class Node;
class World;
class BaseComponent
{
public:
	BaseComponent();
	virtual ~BaseComponent();

public:
	Node * GetOwner();
	void SetOwner(Node* value);

	bool& GetIsActive();
	void SetIsActive(bool value);

	virtual stringT GetTypeName() = 0;

	virtual void Update(double curFrame, double deltaFrame) = 0;

	virtual void OnAddToWorld(World* world) = 0;

	virtual void OnRemoveFromWorld(World* world) = 0;

protected:
	Node * m_Owner;
	bool m_IsActive;
};

