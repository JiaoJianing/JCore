#include "stdafx.h"
#include "World.h"
#include "ResourceManager.h"

World::World(int windowWidth, int windowHeight)
	: m_RootNode(0)
	, m_CameraNode(0)
	, m_CurNodeID(1)
	, m_FreeCamera(0)
	, m_FollowCamera(0)
	, m_TextRender(0)
	, m_PickingNode(0)
	, m_Scene(0)
	, m_Renderer(0)
	, m_PickRenderer(0)
	, m_PostRenderer(0)
	, m_LightRenderer(0)
	, m_SkyboxRenderer(0)
	, m_WindowWidth(windowWidth)
	, m_WindowHeight(windowHeight)
	, m_EnablePostEffect(false)
	, m_EnableLight(false)
	, m_EnableSkybox(false)
	, m_EnableRenderNormal(false)
	, m_FPS(0.0f)
{

}

World::~World()
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

	if (m_TextRender != 0) {
		delete m_TextRender;
		m_TextRender = 0;
	}

	if (m_Scene != 0) {
		delete m_Scene;
		m_Scene = 0;
	}

	if (m_Renderer != 0) {
		delete m_Renderer;
		m_Renderer = 0;
	}

	if (m_PickRenderer != 0) {
		delete m_PickRenderer;
		m_PickRenderer = 0;
	}

	if (m_PostRenderer != 0) {
		delete m_PostRenderer;
		m_PostRenderer = 0;
	}

	if (m_LightRenderer != 0) {
		delete m_LightRenderer;
		m_LightRenderer = 0;
	}

	if (m_SkyboxRenderer != 0) {
		delete m_SkyboxRenderer;
		m_SkyboxRenderer = 0;
	}

	m_Nodes.clear();
	m_NodesToDestroy.clear();
}

bool World::Initialize()
{
	//shader
	ResourceManager::getInstance()->LoadShader("model", "asset/shaders/jcore/model.vs", "asset/shaders/jcore/model.fs");
	ResourceManager::getInstance()->LoadShader("text", "asset/shaders/jcore/text.vs", "asset/shaders/jcore/text.fs");
	ResourceManager::getInstance()->LoadShader("quad", "asset/shaders/jcore/quad.vs", "asset/shaders/jcore/quad.fs");
	ResourceManager::getInstance()->GetShader("quad").use().setInt("texture1", 0);
	ResourceManager::getInstance()->LoadShader("outline", "asset/shaders/jcore/outline.vs", "asset/shaders/jcore/outline.fs");
	ResourceManager::getInstance()->GetShader("outline").use().setVec3("outlineColor", glm::vec3(1.0f));
	ResourceManager::getInstance()->LoadShader("phong", "asset/shaders/jcore/phong.vs", "asset/shaders/jcore/phong.fs");
	ResourceManager::getInstance()->LoadShader("light_debug", "asset/shaders/jcore/light_debug.vs", "asset/shaders/jcore/light_debug.fs");
	ResourceManager::getInstance()->LoadShader("skybox", "asset/shaders/jcore/skybox.vs", "asset/shaders/jcore/skybox.fs");
	ResourceManager::getInstance()->GetShader("skybox").use().setInt("texture_skybox", 0);
	ResourceManager::getInstance()->LoadShader("show_normal", "asset/shaders/jcore/show_normal.vs", "asset/shaders/jcore/show_normal.fs", "asset/shaders/jcore/show_normal.gs");
	ResourceManager::getInstance()->LoadShader("billboard", "asset/shaders/jcore/billboard.vs", "asset/shaders/jcore/billboard.fs", "asset/shaders/jcore/billboard.gs");
	ResourceManager::getInstance()->GetShader("billboard").use().setInt("texture_billboard", 0);
	ResourceManager::getInstance()->LoadShader("particle_system", "asset/shaders/jcore/particle_system.vs", "asset/shaders/jcore/particle_system.fs", "asset/shaders/jcore/particle_system.gs");
	ResourceManager::getInstance()->GetShader("particle_system").use().setInt("texture_particle", 0);

	ResourceManager::getInstance()->LoadTexture("skybox", "asset/resources/skybox", "right.jpg", "left.jpg",
		"top.jpg", "bottom.jpg", "front.jpg","back.jpg");
	ResourceManager::getInstance()->LoadTexture("skybox2", "asset/resources/skybox2", "sp3right.jpg", "sp3left.jpg",
		"sp3top.jpg", "sp3bot.jpg", "sp3front.jpg", "sp3back.jpg");

	//场景管理器
	m_Scene = new Scene();

	//场景根节点
	m_RootNode = new Node(_T("Scene_Root"));

	//相机节点
	m_CameraNode = AddNode(_T("MainCamera"));
	m_FreeCamera = new FreeCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FreeCamera->SetIsActive(true);
	m_FollowCamera = new FollowCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FollowCamera->SetIsActive(false);
	m_CameraNode->AddComponent(m_FreeCamera);
	m_CameraNode->AddComponent(m_FollowCamera);

	m_TextRender = new Text(m_WindowWidth, m_WindowHeight);
	m_TextRender->Load("asset/fonts/msyh.ttf", 36);

	//渲染器
	m_Renderer = new Renderer(m_WindowWidth, m_WindowHeight);
	m_Renderer->Initialize();
	m_PickRenderer = new PickRenderer(m_WindowWidth, m_WindowHeight);
	m_PickRenderer->Initialize();
	m_PostRenderer = new PostEffectRenderer(m_WindowWidth, m_WindowHeight);
	m_PostRenderer->Initialize();
	m_LightRenderer = new LightingRenderer(m_WindowWidth, m_WindowHeight);
	m_LightRenderer->Initialize();
	m_SkyboxRenderer = new SkyBoxRenderer(m_WindowWidth, m_WindowHeight);
	m_SkyboxRenderer->Initialize();

	return true;
}

void World::Update(double curFrame, double deltaFrame)
{
	m_FPS = round(1 / deltaFrame);
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

void World::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	RenderContext context;
	context.GetParamsFromCamera(GetActiveCamera());

	//后期开始
	if (GetEnablePostEffect()) {
		m_PostRenderer->BeginRender();
	}

	//是否调试输出法线

	//是否使用光照
	if (m_EnableLight) {
		m_LightRenderer->Render(m_Scene, &context);
	}
	else {
		m_Renderer->Render(m_Scene, &context);
	}

	//是否输出法线
	if (m_EnableRenderNormal) {
		m_NormalRenderer.Render(m_Scene, &context);
	}

	//渲染天空盒
	if (m_EnableSkybox) {
		if (m_SkyboxRenderer != 0) {
			m_SkyboxRenderer->Render(m_Scene, &context);
		}
	}

	//后期结束
	if (GetEnablePostEffect()) {
		m_PostRenderer->EndRender();
		m_PostRenderer->Render(m_Scene, &context);
	}

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

	//绘制FPS
	std::wstring fps = std::to_wstring(m_FPS);
	m_TextRender->Draw(std::wstring(L"FPS: ") + fps.substr(0, fps.size() - 5), m_WindowWidth - 90.0f, 15.0f, 0.5f, glm::vec3(1.0f));
}

Node* World::AddNode(const stringT& name)
{
	Node* ret = new Node(name);
	ret->SetID(m_CurNodeID++);
	m_Nodes.insert(std::make_pair(ret->GetID(), ret));
	ret->SetParent(m_RootNode);
	ret->SetWorld(this);

	return ret;
}

bool World::RemoveNode(Node* node)
{
	node->OnRemoveFromWorld(this);
	m_NodesToDestroy.push_back(node);

	for (int i = 0; i < node->GetChildCount(); i++) {
		Node* child = node->GetChildAt(i);
		RemoveNode(child);
	}

	return true;
}

bool World::RemoveNode(unsigned long id)
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

Node* World::FindNode(const stringT& name)
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

Node* World::GetRootNode()
{
	return m_RootNode;
}

Node* World::PickNode(unsigned int x, unsigned int y)
{
	//拾取系统
	RenderContext context;
	context.GetParamsFromCamera(GetActiveCamera());
	m_PickRenderer->Render(m_Scene, &context);

	Node* ret = 0;
	PickInfo pick = m_PickRenderer->Pick(x, y);
	if (m_Nodes.find(pick.nodeID) != m_Nodes.end() && m_Nodes[pick.nodeID]->GetPickable()) {
		ret = m_Nodes[pick.nodeID];
	}

	if (m_PickingNode != ret) {
		if (m_PickingNode != 0) {
			m_PickingNode->SetHighLight(false);
		}
		m_PickingNode = ret;
		if (m_PickingNode != 0) {
			m_PickingNode->SetHighLight(true);
		}
	}
	return ret;
}

void World::OnMouseMove(double xPos, double yPos)
{
	GetActiveCamera()->OnMouseMove(xPos, yPos);
}

void World::OnMouseDown()
{
	GetActiveCamera()->OnMouseDown();
}

void World::OnMouseUp()
{
	GetActiveCamera()->OnMouseUp();
}

void World::OnKeyboard(int key)
{
	GetActiveCamera()->OnKeyboard(key);
}

void World::OnMouseScroll(double xOffset, double yOffset)
{
	GetActiveCamera()->OnMouseScroll(xOffset, yOffset);
}

void World::OnResize(int width, int height)
{
	m_FreeCamera->Resize(width, height);
	m_FollowCamera->Resize(width, height);

	m_Renderer->Resize(width, height);
	m_PickRenderer->Resize(width, height);
	m_PostRenderer->Resize(width, height);
	m_LightRenderer->Resize(width, height);
	m_SkyboxRenderer->Resize(width, height);
}

void World::ToFree(const glm::vec3& position)
{
	m_FreeCamera->SetIsActive(true);
	m_FreeCamera->SetPos(position);
	m_FollowCamera->SetIsActive(false);
}

void World::ToFollow(Node* node)
{
	m_FreeCamera->SetIsActive(false);
	m_FollowCamera->SetIsActive(true);
	m_FollowCamera->SetFollowNode(node);
}

CameraComponent* World::GetActiveCamera()
{
	if (m_FreeCamera->GetIsActive()) {
		return m_FreeCamera;
	}
	if (m_FollowCamera->GetIsActive()) {
		return m_FollowCamera;
	}

	return 0;
}

Scene* World::GetScene()
{
	return m_Scene;
}

bool World::GetEnablePostEffect()
{
	return m_EnablePostEffect;
}

void World::SetEnablePostEffect(bool value)
{
	m_EnablePostEffect = value;
}

bool World::GetEnableLight()
{
	return m_EnableLight;
}

void World::SetEnableLight(bool value)
{
	m_EnableLight = value;
}

bool World::GetEnableSkybox()
{
	return m_EnableSkybox;
}

void World::SetEnableSkybox(bool value)
{
	m_EnableSkybox = value;
}

bool World::GetEnableRenderNormal()
{
	return m_EnableRenderNormal;
}

void World::SetEnableRenderNormal(bool value)
{
	m_EnableRenderNormal = value;
}
