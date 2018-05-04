#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

Scene::Scene(int windowWidth, int windowHeight)
	: m_RootNode(0)
	, m_CurNodeID(0)
	, m_Camera(0)
	, m_TextRender(0)
	, m_Effects(0)
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

	if (m_Camera != 0) {
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_TextRender != 0) {
		delete m_TextRender;
		m_TextRender = 0;
	}

	if (m_Effects != 0) {
		delete m_Effects;
		m_Effects = 0;
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

	m_RootNode = new Node("Scene_Root");

	m_Camera = new Camera(m_WindowWidth, m_WindowHeight);

	m_TextRender = new Text(m_WindowWidth, m_WindowHeight);
	m_TextRender->Load("asset/fonts/msyh.ttf", 36);

	m_Effects = new PostProcessor(ResourceManager::getInstance()->GetShader("post"), m_WindowWidth, m_WindowHeight);
}

void Scene::Update(double curFrame, double deltaFrame)
{
	m_Camera->Update(curFrame, deltaFrame);
	glm::mat4 view = glm::lookAt(m_Camera->GetPos(), m_Camera->GetPos() + m_Camera->GetTarget(), m_Camera->GetUp());
	glm::mat4 projection = glm::perspective(m_Camera->GetFov(), (float)m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	ResourceManager::getInstance()->GetShader("cube").use().setMatrix4("view", view);
	ResourceManager::getInstance()->GetShader("cube").setMatrix4("projection", projection);
	ResourceManager::getInstance()->GetShader("model").use().setMatrix4("view", view);
	ResourceManager::getInstance()->GetShader("model").setMatrix4("projection", projection);

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
}

void Scene::Render()
{
	m_Effects->BeginRender();

	m_RootNode->Render();

	m_Effects->EndRender();

	m_Effects->Render();

	m_TextRender->Draw(L"Welcome to JCore", 15.0f, 15.0f, 1.0f, glm::vec3(1.0f, 0.5f, 0.5f));
}

Node* Scene::AddNode(const std::string& name)
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
		ret->second->SetParent(0);
		m_Nodes.erase(ret);
	}
	else {
		return false;
	}

	return true;
}

Node* Scene::FindNode(const std::string& name)
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

void Scene::OnMouseMove(double xPos, double yPos)
{
	m_Camera->OnMouseMove(xPos, yPos);
}

void Scene::OnMouseDown()
{
	m_Camera->OnMouseDown();
}

void Scene::OnMouseUp()
{
	m_Camera->OnMouseUp();
}

void Scene::OnKeyboard(int key)
{
	m_Camera->OnKeyboard(key);
}

void Scene::OnMouseScroll(double xOffset, double yOffset)
{
	m_Camera->OnMouseScroll(xOffset, yOffset);
}
