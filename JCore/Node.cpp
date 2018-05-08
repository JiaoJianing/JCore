#include "stdafx.h"
#include "Node.h"
#include "ResourceManager.h"

Node::Node(const stringT& name)
	: m_Parent(0)
	, m_ID(1)
	, m_Name(name)
	, m_TransformDirty(true)
	, m_Color(1.0f)
	, m_HighLight(false)
	, m_HighLightColor(1.0f, 1.0f, 0.0f)
	, m_Pickable(true)
	, m_World(0)
{
}

Node::Node(const Node& other)
{

}

Node& Node::operator=(const Node& other)
{
	return *this;
}

Node::~Node()
{
	for (std::vector<BaseComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		if ((*it) != 0) {
			delete *it;
			*it = 0;
		}
	}
}

void Node::Update(double curFrame, double deltaFrame)
{
	for (std::vector<BaseComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		if ((*it) != 0) {
			(*it)->Update(curFrame, deltaFrame);
		}
	}

	//调用子节点的Update
	for (int i = 0; i < m_Children.size(); i++) {
		m_Children[i]->Update(curFrame, deltaFrame);
	}

	m_TransformDirty = false;
}

Node * Node::GetParent()
{
	return m_Parent;
}

void Node::SetParent(Node* parent)
{
	if (m_Parent == parent)
	{
		return;
	}

	if (m_Parent != 0) {
		m_Parent->removeChild(this);
	}

	m_Parent = parent;

	if (m_Parent != 0) {
		m_Parent->addChild(this);
	}
}

const glm::mat4& Node::GetWorldTransform()
{
	return m_WorldTransform;
}

void Node::SetWorldTransform(const glm::mat4& value)
{
	if (m_WorldTransform != value) {
		m_WorldTransform = value;

		m_TransformDirty = true;
	}
}

stringT& Node::GetName()
{
	return m_Name;
}

void Node::SetName(const stringT& name)
{
	m_Name = name;
}

unsigned long Node::GetID()
{
	return m_ID;
}

void Node::SetID(unsigned long id)
{
	m_ID = id;
}

glm::vec3& Node::GetColor()
{
	return m_Color;
}

void Node::SetColor(const glm::vec3& color)
{
	m_Color = color;
}

int Node::GetChildCount()
{
	return m_Children.size();
}

Node* Node::GetChildAt(int index)
{
	return m_Children[index];
}

bool Node::GetTransformDirty()
{
	return m_TransformDirty;
}

void Node::SetTransformDirty(bool value)
{
	m_TransformDirty = value;
}

void Node::AddComponent(BaseComponent* component)
{
	for (std::vector<BaseComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		if ((*it)->GetTypeName() == component->GetTypeName()) {
			throw "Same Component has existed";
		}
	}

	m_Components.push_back(component);
	component->SetOwner(this);
	component->OnAddToWorld(GetWorld());
}

bool Node::GetHighLight()
{
	return m_HighLight;
}

void Node::SetHighLight(bool value)
{
	if (value != m_HighLight) {
		m_HighLight = value;
	}
}

bool Node::GetPickable()
{
	return m_Pickable;
}

void Node::SetPickable(bool value)
{
	m_Pickable = value;
}

const glm::vec3& Node::GetHighLightColor() const
{
	return m_HighLightColor;
}

void Node::SetHighLightColor(const glm::vec3& color)
{
	if (m_HighLightColor != color) {
		m_HighLightColor = color;
	}
}

World* Node::GetWorld()
{
	return m_World;
}

void Node::SetWorld(World* world)
{
	m_World = world;
}

void Node::OnRemoveFromWorld(World* world)
{
	for (std::vector<BaseComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		if (*it != 0) {
			(*it)->OnRemoveFromWorld(world);
		}
	}
	this->SetWorld(0);
}

void Node::removeChild(Node* node)
{
	std::vector<Node*>::iterator ret = m_Children.begin();
	for (; ret != m_Children.end();) {
		if ((*ret) == node) {
			ret = m_Children.erase(ret);
		}
		else {
			ret++;
		}
	}
}

void Node::addChild(Node* node)
{
	if (node == 0) {
		return;
	}

	//防止重复添加
	for (int i = 0; i < m_Children.size(); i++) {
		if (m_Children[i] == node) {
			return;
		}
	}

	m_Children.push_back(node);
}
