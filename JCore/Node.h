#pragma once
#include <glm.hpp>
#include <vector>
#include "SRTTransform.h"
#include "BaseComponent.h"
#include "Shader.h"

//抽象节点类
class Node
{
	friend class Scene;
public:
	void Update(double curFrame, double deltaFrame);

	void Render();

	void Render(Shader shader);

public:
	Node * GetParent();
	void SetParent(Node* parent);

	const glm::mat4& GetWorldTransform();
	void SetWorldTransform(const glm::mat4& value);

	stringT& GetName();
	void SetName(const stringT& name);

	unsigned long GetID();
	void SetID(unsigned long id);

	glm::vec3& GetColor();
	void SetColor(const glm::vec3& color);

	void SetHightLight(bool value);

	int GetChildCount();
	Node* GetChildAt(int index);

	bool GetTransformDirty();
	void SetTransformDirty(bool value);

	void AddComponent(BaseComponent* component);
	template<typename T>
	T* FindComponent();

private:
	Node(const stringT& name);
	Node(const Node& other);
	Node& operator=(const Node& other);
	~Node();

private:
	void removeChild(Node* node);
	void addChild(Node* node);

private:
	Node* m_Parent;
	std::vector<Node*> m_Children;
	glm::mat4 m_WorldTransform;
	unsigned long m_ID; //ID从1开始
	stringT m_Name;
	glm::vec3 m_Color;

	std::vector<BaseComponent*> m_Components;

	bool m_TransformDirty;
};

template<typename T>
T* Node::FindComponent() {
	for (std::vector<BaseComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		T* ret = dynamic_cast<T*>(*it);
		if (ret != 0) {
			return ret;
		}
	}

	return 0;
}

