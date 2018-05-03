#pragma once
#include <glm.hpp>
#include <vector>
#include "Cube.h"
#include "SRTTransform.h"

//抽象节点类
class Node
{
	friend class Scene;
public:
	void Update(double curFrame, double deltaFrame);

	void Render();

public:
	Node * GetParent();
	void SetParent(Node* parent);

	const glm::mat4& GetWorldTransform();
	void SetWorldTransform(const glm::mat4& value);

	std::string& GetName();
	void SetName(const std::string& name);

	unsigned long GetID();
	void SetID(unsigned long id);

	glm::vec3& GetColor();
	void SetColor(const glm::vec3& color);

	int GetChildCount();
	Node* GetChildAt(int index);

	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetRotate(float rotateX, float rotateY, float rotateZ);
	void SetTranslate(float translateX, float translateY, float translateZ);

	bool GetTransformDirty();
	void SetTransformDirty(bool value);

private:
	Node(const std::string& name);
	Node(const Node& other);
	Node& operator=(const Node& other);
	~Node();

private:
	void removeChild(Node* node);
	void addChild(Node* node);

	//更新世界矩阵
	void updateWorldTransform();

private:
	Node* m_Parent;
	std::vector<Node*> m_Children;
	glm::mat4 m_WorldTransform;
	unsigned long m_ID;
	std::string m_Name;
	glm::vec3 m_Color;

	bool m_TransformDirty;
	SRTTransform m_SRT;

	Cube m_Cube;
};

