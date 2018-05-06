#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

Scene::Scene(int windowWidth, int windowHeight)
	: m_RootNode(0)
	, m_CurNodeID(1)
	, m_FreeCamera(0)
	, m_FollowCamera(0)
	, m_TextRender(0)
	, m_Effects(0)
	, m_PickingSys(0)
	, m_TestQuad(0)
	, m_PickingNode(0)
	, m_WindowWidth(windowWidth)
	, m_WindowHeight(windowHeight)
{
}

Scene::~Scene()
{
	for (std::map<unsigned long, Node*>::iterator it = m_Nodes.begin(); it != m_Nodes.end(); it++) {
		if (it->second != 0) {
			delete it->second;
			it->second = 0;
		}
	}

	if (m_RootNode != 0) {
		delete m_RootNode;
		m_RootNode = 0;
	}

	if (m_FreeCamera != 0) {
		delete m_FreeCamera;
		m_FreeCamera = 0;
	}

	if (m_FollowCamera != 0) {
		delete m_FollowCamera;
		m_FollowCamera = 0;
	}

	if (m_TextRender != 0) {
		delete m_TextRender;
		m_TextRender = 0;
	}

	if (m_Effects != 0) {
		delete m_Effects;
		m_Effects = 0;
	}

	if (m_PickingSys != 0) {
		delete m_PickingSys;
		m_PickingSys = 0;
	}

	if (m_TestQuad != 0) {
		delete m_TestQuad;
		m_TestQuad = 0;
	}

	m_Nodes.clear();
	m_NodesToDestroy.clear();
}

void Scene::Initialize()
{
	//shader
	ResourceManager::getInstance()->LoadShader("cube", "asset/shaders/jcore/cube.vs", "asset/shaders/jcore/cube.fs");
	ResourceManager::getInstance()->LoadShader("model", "asset/shaders/jcore/model.vs", "asset/shaders/jcore/model.fs");
	ResourceManager::getInstance()->LoadShader("text", "asset/shaders/jcore/text.vs", "asset/shaders/jcore/text.fs");
	ResourceManager::getInstance()->LoadShader("post", "asset/shaders/jcore/post.vs", "asset/shaders/jcore/post.fs");
	ResourceManager::getInstance()->LoadShader("pick", "asset/shaders/jcore/pick.vs", "asset/shaders/jcore/pick.fs");
	ResourceManager::getInstance()->LoadShader("quad", "asset/shaders/jcore/quad.vs", "asset/shaders/jcore/quad.fs");

	m_RootNode = new Node(_T("Scene_Root"));

	m_FreeCamera = new FreeCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FreeCamera->SetIsActive(true);
	m_FollowCamera = new FollowCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FollowCamera->SetIsActive(false);

	m_TextRender = new Text(m_WindowWidth, m_WindowHeight);
	m_TextRender->Load("asset/fonts/msyh.ttf", 36);

	m_Effects = new PostProcessor(ResourceManager::getInstance()->GetShader("post"), m_WindowWidth, m_WindowHeight);

	m_PickingSys = new PickingSystem(m_WindowWidth, m_WindowHeight);

	m_TestQuad = new Quad();
	ResourceManager::getInstance()->GetShader("quad").use().setInt("texture1", 0);
}

void Scene::Update(double curFrame, double deltaFrame)
{
	glm::mat4 worldTransform;
	if (m_RootNode != 0) {
		m_RootNode->SetWorldTransform(worldTransform);
		m_RootNode->Update(curFrame, deltaFrame);
	}

	//从节点树中清除
	for (std::vector<Node*>::iterator it = m_NodesToDestroy.begin(); it != m_NodesToDestroy.end(); it++) {
		RemoveNode((*it)->GetID());
	}
	//从内存清除
	for (std::vector<Node*>::iterator it = m_NodesToDestroy.begin(); it != m_NodesToDestroy.end(); it++) {
		delete *it;
	}

	m_NodesToDestroy.clear();

	GetActiveCamera()->Update(curFrame, deltaFrame);
	glm::mat4 view = GetActiveCamera()->GetViewTransform();
	glm::mat4 projection = GetActiveCamera()->GetProjectionTransform();
	ResourceManager::getInstance()->GetShader("cube").use().setMatrix4("view", view);
	ResourceManager::getInstance()->GetShader("cube").setMatrix4("projection", projection);
	ResourceManager::getInstance()->GetShader("model").use().setMatrix4("view", view);
	ResourceManager::getInstance()->GetShader("model").setMatrix4("projection", projection);
	ResourceManager::getInstance()->GetShader("pick").use().setMatrix4("view", view);
	ResourceManager::getInstance()->GetShader("pick").setMatrix4("projection", projection);
	ResourceManager::getInstance()->GetShader("pick").setMatrix4("model", glm::mat4());
}

void Scene::Render()
{
	//后期
	m_Effects->BeginRender();

	m_RootNode->Render();

	m_Effects->EndRender();

	m_Effects->Render();

	//文字
	if (m_FreeCamera->GetIsActive()) {
		m_TextRender->Draw(std::wstring(L"自由相机"), 15.0f, 15.0f, 0.5f, glm::vec3(1.0f, 0.5f, 0.5f));
	}
	else if (m_FollowCamera->GetIsActive()) {
		m_TextRender->Draw(std::wstring(L"跟随相机: ") + m_FollowCamera->GetFollowNode()->GetName(), 15.0f, 15.0f, 0.5f, glm::vec3(1.0f, 0.5f, 0.5f));
	}

	if (m_PickingNode != 0) {
		m_TextRender->Draw(std::wstring(L"当前拾取: ") + m_PickingNode->GetName(), 15.0f, 35.0f, 0.5f, glm::vec3(1.0f, 0.5f, 0.5f));
	}
}

Node* Scene::AddNode(const stringT& name)
{
	Node* ret = new Node(name);
	ret->SetID(m_CurNodeID++);
	m_Nodes.insert(std::make_pair(ret->GetID(), ret));
	ret->SetParent(m_RootNode);

	return ret;
}

bool Scene::RemoveNode(Node* node)
{
	m_NodesToDestroy.push_back(node);

	for (int i = 0; i < node->GetChildCount(); i++) {
		Node* child = node->GetChildAt(i);
		RemoveNode(child);
	}

	return true;
}

bool Scene::RemoveNode(unsigned long id)
{
	std::map<unsigned long, Node*>::iterator ret = m_Nodes.find(id);
	if (ret != m_Nodes.end()) {
		//若清除的是点前follow相机跟随的节点，则切换状态
		if (m_FollowCamera->GetFollowNode() == ret->second) {
			m_FollowCamera->SetFollowNode(0);
			m_FollowCamera->SetIsActive(false);
			m_FreeCamera->SetIsActive(true);
		}
		ret->second->SetParent(0);
		m_Nodes.erase(ret);
	}
	else {
		return false;
	}

	return true;
}

Node* Scene::FindNode(const stringT& name)
{
	for (std::map<unsigned long, Node*>::iterator it = m_Nodes.begin(); it != m_Nodes.end(); it++) {
		if (it->second != 0) {
			if (it->second->GetName() == name) {
				return it->second;
			}
		}
	}
	return 0;
}

Node* Scene::GetRootNode()
{
	return m_RootNode;
}

Node* Scene::PickNode(unsigned int x, unsigned int y)
{
	//拾取系统
	m_PickingSys->BeginRender();

	m_RootNode->Render(ResourceManager::getInstance()->GetShader("pick"));

	m_PickingSys->EndRender();

	Node* ret = 0;
	PickInfo pick = m_PickingSys->Pick(x, y);
	if (m_Nodes.find(pick.nodeID) != m_Nodes.end()) {
		ret = m_Nodes[pick.nodeID];
	}

	m_PickingNode = ret;
	return ret;
}

void Scene::OnMouseMove(double xPos, double yPos)
{
	GetActiveCamera()->OnMouseMove(xPos, yPos);
}

void Scene::OnMouseDown()
{
	GetActiveCamera()->OnMouseDown();
}

void Scene::OnMouseUp()
{
	GetActiveCamera()->OnMouseUp();
}

void Scene::OnKeyboard(int key)
{
	GetActiveCamera()->OnKeyboard(key);
}

void Scene::OnMouseScroll(double xOffset, double yOffset)
{
	GetActiveCamera()->OnMouseScroll(xOffset, yOffset);
}

void Scene::ToFree(const glm::vec3& position)
{
	m_FreeCamera->SetIsActive(true);
	m_FreeCamera->SetPos(position);
	m_FollowCamera->SetIsActive(false);
}

void Scene::ToFollow(Node* node)
{
	m_FreeCamera->SetIsActive(false);
	m_FollowCamera->SetIsActive(true);
	m_FollowCamera->SetFollowNode(node);
}

CameraComponent* Scene::GetActiveCamera()
{
	if (m_FreeCamera->GetIsActive()) {
		return m_FreeCamera;
	}
	if (m_FollowCamera->GetIsActive()) {
		return m_FollowCamera;
	}

	return 0;
}
