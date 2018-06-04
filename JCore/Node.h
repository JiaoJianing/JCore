#pragma once
#include <glm.hpp>
#include <vector>
#include "SRTTransform.h"
#include "BaseComponent.h"
#include "Shader.h"

//抽象节点类
class Node
{
	friend class World;
public:
	void Update(double curFrame, double deltaFrame);

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

	int GetChildCount();
	Node* GetChildAt(int index);

	bool GetTransformDirty();
	void SetTransformDirty(bool value);

	void AddComponent(BaseComponent* component);
	template<typename T>
	T* FindComponent();

	bool GetHighLight();
	void SetHighLight(bool value);

	bool GetPickable();
	void SetPickable(bool value);

	const glm::vec3& GetHighLightColor() const;
	void SetHighLightColor(const glm::vec3& color);

	World* GetWorld();
	void SetWorld(World* world);

	void OnRemoveFromWorld(World* world);

	glm::vec3& GetFrontDir();
	void SetFrontDir(const glm::vec3& value);

	glm::vec3& GetUpDir();
	void SetUpDir(const glm::vec3& value);

	int GetCurrentAnimation();
	void SetCurrentAnimation(int value);

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
	glm::vec3 m_FrontDir;
	glm::vec3 m_UpDir;
	int m_CurrentAnimation;

	std::vector<BaseComponent*> m_Components;

	bool m_TransformDirty;

	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	bool m_Pickable;

	World* m_World;
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

