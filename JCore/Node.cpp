#include "stdafx.h"
#include "Node.h"
#include "ResourceManager.h"

Node::Node(const std::string& name)
	: m_Parent(0)
	, m_ID(0)
	, m_Name(name)
	, m_TransformDirty(true)
	, m_Color(1.0f)
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
}

void Node::Update(double curFrame, double deltaFrame)
{
	updateWorldTransform();

	//调用子节点的Update
	for (int i = 0; i < m_Children.size(); i++) {
		m_Children[i]->Update(curFrame, deltaFrame);
	}
}

void Node::Render()
{
	m_Cube.Draw(GetWorldTransform());

	for (int i = 0; i < m_Children.size(); i++) {
		m_Children[i]->Render();
	}
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

std::string& Node::GetName()
{
	return m_Name;
}

void Node::SetName(const std::string& name)
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
	m_Cube.SetColor(color);
}

int Node::GetChildCount()
{
	return m_Children.size();
}

Node* Node::GetChildAt(int index)
{
	return m_Children[index];
}

void Node::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_SRT.SetScale(glm::vec3(scaleX, scaleY, scaleZ));
	SetTransformDirty(true);
}

void Node::SetRotate(float rotateX, float rotateY, float rotateZ)
{
	m_SRT.SetRotate(glm::vec3(rotateX, rotateY, rotateZ));	
	SetTransformDirty(true);
}

void Node::SetTranslate(float translateX, float translateY, float translateZ)
{
	m_SRT.SetTranslation(glm::vec3(translateX, translateY, translateZ));
	SetTransformDirty(true);
}

bool Node::GetTransformDirty()
{
	return m_TransformDirty;
}

void Node::SetTransformDirty(bool value)
{
	m_TransformDirty = value;
	for (int i = 0; i < m_Children.size(); i++) {
		m_Children[i]->SetTransformDirty(true);
	}
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

void Node::updateWorldTransform()
{
	if (m_TransformDirty)
	{
		if (m_Parent != 0) {
			m_WorldTransform = m_Parent->GetWorldTransform() * m_SRT.GetTransformMatrix();
		}

		m_TransformDirty = false;
	}
}
