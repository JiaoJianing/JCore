#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include "Camera.h"
#include "Text.h"
#include "PostProcessor.h"

//场景管理类
class Scene
{
public:
	Scene(int windowWidth, int windowHeight);
	~Scene();

	void Initialize();

	void Update(double curFrame, double deltaFrame);

	void Render();

	//场景中节点的name可能重复，但ID唯一
	Node* AddNode(const std::string& name);

	//删除节点，先将节点及其子节点移动到m_NodesToDestroy，在下一帧统一删除
	bool RemoveNode(Node* node);

	//真正的删除一个节点
	bool RemoveNode(unsigned long id);

	//找到场景中第一个名字匹配的节点
	Node* FindNode(const std::string& name);

	//获得场景根节点
	Node* GetRootNode();

	//UI事件
	void OnMouseMove(double xPos, double yPos);

	void OnMouseDown();

	void OnMouseUp();

	void OnKeyboard(int key);

	void OnMouseScroll(double xOffset, double yOffset);

private:
	std::map<unsigned long, Node*> m_Nodes;//场景中所有的节点，使用map便于查询
	std::vector<Node*> m_NodesToDestroy;//将要删除的节点，一般在下一帧删除
	Node* m_RootNode;//场景根节点
	unsigned long m_CurNodeID;//管理节点id
	Camera* m_Camera;
	int m_WindowWidth;
	int m_WindowHeight;
};

