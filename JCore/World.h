#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include "Text.h"
#include "FreeCameraComponent.h"
#include "FollowCameraComponent.h"
#include "Scene.h"
#include "Renderer.h"
#include "PickRenderer.h"
#include "PostEffectRenderer.h"

class World
{
public:
	World(int windowWidth, int windowHeight);
	~World();

	void Initialize();

	void Update(double curFrame, double deltaFrame);

	void Render();

	//�����нڵ��name�����ظ�����IDΨһ
	Node* AddNode(const stringT& name);

	//ɾ���ڵ㣬�Ƚ��ڵ㼰���ӽڵ��ƶ���m_NodesToDestroy������һ֡ͳһɾ��
	bool RemoveNode(Node* node);

	//������ɾ��һ���ڵ�
	bool RemoveNode(unsigned long id);

	//�ҵ������е�һ������ƥ��Ľڵ�
	Node* FindNode(const stringT& name);

	//��ó������ڵ�
	Node* GetRootNode();

	//ʰȡ
	Node* PickNode(unsigned int x, unsigned int y);

	//UI�¼�
	void OnMouseMove(double xPos, double yPos);

	void OnMouseDown();

	void OnMouseUp();

	void OnKeyboard(int key);

	void OnMouseScroll(double xOffset, double yOffset);

	void ToFree(const glm::vec3& position);

	void ToFollow(Node* node);

	CameraComponent* GetActiveCamera();

	Scene* GetScene();

	bool GetEnablePostEffect();
	void SetEnablePostEffect(bool value);

private:
	std::map<unsigned long, Node*> m_Nodes;//���������еĽڵ㣬ʹ��map���ڲ�ѯ
	std::vector<Node*> m_NodesToDestroy;//��Ҫɾ���Ľڵ㣬һ������һ֡ɾ��
	Node* m_RootNode;//�������ڵ�
	unsigned long m_CurNodeID;//����ڵ�id ��1��ʼ
	FreeCameraComponent* m_FreeCamera;
	FollowCameraComponent* m_FollowCamera;
	int m_WindowWidth;
	int m_WindowHeight;
	bool m_EnablePostEffect;

	Text* m_TextRender;

	Node* m_PickingNode;

	Scene* m_Scene;
	Renderer* m_Renderer;
	PickRenderer* m_PickRenderer;
	PostEffectRenderer* m_PostRenderer;
};

