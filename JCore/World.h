#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include "TextRenderer.h"
#include "FreeCameraComponent.h"
#include "FollowCameraComponent.h"
#include "Scene.h"
#include "MainRenderer.h"
#include "PickRenderer.h"
#include "PostEffectRenderer.h"
#include "SnowParticleSystemComponent.h"

class World
{
public:
	World(int windowWidth, int windowHeight);
	~World();

	bool Initialize();

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

	void OnResize(int width, int height);

	void ToFree(const glm::vec3& position);

	void ToFollow(Node* node);

	void ToFlyMode(bool value);
	bool& GetFlyMode();

	CameraComponent* GetActiveCamera();

	CameraComponent* GetFreeCamera();

	CameraComponent* GetFollowCamera();

	Scene* GetScene();

	float GetHeightAt(const glm::vec3& position);

	bool GetEnablePostEffect();
	void SetEnablePostEffect(bool value);

	bool& GetEnableLight();
	void SetEnableLight(bool value);

	bool& GetEnableSkybox();
	void SetEnableSkybox(bool value);

	bool& GetEnableRenderNormal();
	void SetEnableRenderNormal(bool value);

	bool& GetEnableSnow();
	void SetEnableSnow(bool value);

	int GetWindowWidth();
	int GetWindowHeight();

	float& GetFreeCamKeySensitivity();

	glm::vec3& GetSunDirection();
	void SetSunDirection(const glm::vec3& value);

	float& GetWaterHeight();

private:
	std::map<unsigned long, Node*> m_Nodes;//���������еĽڵ㣬ʹ��map���ڲ�ѯ
	std::vector<Node*> m_NodesToDestroy;//��Ҫɾ���Ľڵ㣬һ������һ֡ɾ��
	Node* m_RootNode;//�������ڵ�
	Node* m_CameraNode;//������Ľڵ�
	unsigned long m_CurNodeID;//����ڵ�id ��1��ʼ
	FreeCameraComponent* m_FreeCamera;
	FollowCameraComponent* m_FollowCamera;
	int m_WindowWidth;
	int m_WindowHeight;
	float m_FPS;

	TextRenderer* m_TextRender;

	Node* m_PickingNode;
	SRTTransformComponent* m_PickingNodeSRT;

	Scene* m_Scene;
	MainRenderer* m_Renderer;

	double m_CurrentFrame;
	double m_DeltaFrame;

	bool m_IsDragging;
	float m_PickNodeScreenZ;

	SnowParticleSystemComponent* m_SnowCmp;
};

