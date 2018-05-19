#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include "Text.h"
#include "FreeCameraComponent.h"
#include "FollowCameraComponent.h"
#include "Scene.h"
#include "MainRenderer.h"
#include "PickRenderer.h"
#include "PostEffectRenderer.h"

class World
{
public:
	World(int windowWidth, int windowHeight);
	~World();

	bool Initialize();

	void Update(double curFrame, double deltaFrame);

	void Render();

	//场景中节点的name可能重复，但ID唯一
	Node* AddNode(const stringT& name);

	//删除节点，先将节点及其子节点移动到m_NodesToDestroy，在下一帧统一删除
	bool RemoveNode(Node* node);

	//真正的删除一个节点
	bool RemoveNode(unsigned long id);

	//找到场景中第一个名字匹配的节点
	Node* FindNode(const stringT& name);

	//获得场景根节点
	Node* GetRootNode();

	//拾取
	Node* PickNode(unsigned int x, unsigned int y);

	//UI事件
	void OnMouseMove(double xPos, double yPos);

	void OnMouseDown();

	void OnMouseUp();

	void OnKeyboard(int key);

	void OnMouseScroll(double xOffset, double yOffset);

	void OnResize(int width, int height);

	void ToFree(const glm::vec3& position);

	void ToFollow(Node* node);

	CameraComponent* GetActiveCamera();

	Scene* GetScene();

	bool GetEnablePostEffect();
	void SetEnablePostEffect(bool value);

	bool GetEnableLight();
	void SetEnableLight(bool value);

	bool GetEnableSkybox();
	void SetEnableSkybox(bool value);

	bool GetEnableRenderNormal();
	void SetEnableRenderNormal(bool value);

private:
	std::map<unsigned long, Node*> m_Nodes;//场景中所有的节点，使用map便于查询
	std::vector<Node*> m_NodesToDestroy;//将要删除的节点，一般在下一帧删除
	Node* m_RootNode;//场景根节点
	Node* m_CameraNode;//放相机的节点
	unsigned long m_CurNodeID;//管理节点id 从1开始
	FreeCameraComponent* m_FreeCamera;
	FollowCameraComponent* m_FollowCamera;
	int m_WindowWidth;
	int m_WindowHeight;
	float m_FPS;

	Text* m_TextRender;

	Node* m_PickingNode;

	Scene* m_Scene;
	MainRenderer* m_Renderer;
};

