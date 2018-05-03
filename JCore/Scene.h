#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include "Camera.h"
#include "Text.h"
#include "PostProcessor.h"

//����������
class Scene
{
public:
	Scene(int windowWidth, int windowHeight);
	~Scene();

	void Initialize();

	void Update(double curFrame, double deltaFrame);

	void Render();

	//�����нڵ��name�����ظ�����IDΨһ
	Node* AddNode(const std::string& name);

	//ɾ���ڵ㣬�Ƚ��ڵ㼰���ӽڵ��ƶ���m_NodesToDestroy������һ֡ͳһɾ��
	bool RemoveNode(Node* node);

	//������ɾ��һ���ڵ�
	bool RemoveNode(unsigned long id);

	//�ҵ������е�һ������ƥ��Ľڵ�
	Node* FindNode(const std::string& name);

	//��ó������ڵ�
	Node* GetRootNode();

	//UI�¼�
	void OnMouseMove(double xPos, double yPos);

	void OnMouseDown();

	void OnMouseUp();

	void OnKeyboard(int key);

	void OnMouseScroll(double xOffset, double yOffset);

private:
	std::map<unsigned long, Node*> m_Nodes;//���������еĽڵ㣬ʹ��map���ڲ�ѯ
	std::vector<Node*> m_NodesToDestroy;//��Ҫɾ���Ľڵ㣬һ������һ֡ɾ��
	Node* m_RootNode;//�������ڵ�
	unsigned long m_CurNodeID;//����ڵ�id
	Camera* m_Camera;
	int m_WindowWidth;
	int m_WindowHeight;
};

