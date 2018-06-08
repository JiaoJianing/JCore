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
	, m_PickingNodeSRT(0)
	, m_Scene(0)
	, m_Renderer(0)
	, m_WindowWidth(windowWidth)
	, m_WindowHeight(windowHeight)
	, m_FPS(0.0f)
	, m_IsDragging(false)
	, m_PickNodeScreenZ(0.0f)
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

	m_Nodes.clear();
	m_NodesToDestroy.clear();
}

bool World::Initialize()
{
	//shader
	ResourceManager::getInstance()->LoadShader("model", "asset/shaders/jcore/model.vs", "asset/shaders/jcore/model.fs");
	ResourceManager::getInstance()->LoadShader("model_animation", "asset/shaders/jcore/model_animation.vs", "asset/shaders/jcore/model_animation.fs");
	ResourceManager::getInstance()->LoadShader("text", "asset/shaders/jcore/text.vs", "asset/shaders/jcore/text.fs");
	ResourceManager::getInstance()->LoadShader("quad", "asset/shaders/jcore/quad.vs", "asset/shaders/jcore/quad.fs");
	ResourceManager::getInstance()->GetShader("quad").use().setInt("texture1", 0);
	ResourceManager::getInstance()->LoadShader("outline", "asset/shaders/jcore/outline.vs", "asset/shaders/jcore/outline.fs");
	ResourceManager::getInstance()->GetShader("outline").use().setVec3("outlineColor", glm::vec3(1.0f));
	ResourceManager::getInstance()->LoadShader("outline_animation", "asset/shaders/jcore/outline_animation.vs", "asset/shaders/jcore/outline_animation.fs");
	ResourceManager::getInstance()->GetShader("outline_animation").use().setVec3("outlineColor", glm::vec3(1.0f));
	ResourceManager::getInstance()->LoadShader("phong", "asset/shaders/jcore/phong.vs", "asset/shaders/jcore/phong.fs");
	ResourceManager::getInstance()->GetShader("phong").use().setInt("texture_shadowmap", 4);
	ResourceManager::getInstance()->LoadShader("light_debug", "asset/shaders/jcore/light_debug.vs", "asset/shaders/jcore/light_debug.fs");
	ResourceManager::getInstance()->LoadShader("skybox", "asset/shaders/jcore/skybox.vs", "asset/shaders/jcore/skybox.fs");
	ResourceManager::getInstance()->GetShader("skybox").use().setInt("texture_skybox", 0);
	ResourceManager::getInstance()->LoadShader("show_normal", "asset/shaders/jcore/show_normal.vs", "asset/shaders/jcore/show_normal.fs", "asset/shaders/jcore/show_normal.gs");
	ResourceManager::getInstance()->LoadShader("show_normal_animation", "asset/shaders/jcore/show_normal_animation.vs", "asset/shaders/jcore/show_normal.fs", "asset/shaders/jcore/show_normal.gs");
	ResourceManager::getInstance()->LoadShader("billboard", "asset/shaders/jcore/billboard.vs", "asset/shaders/jcore/billboard.fs", "asset/shaders/jcore/billboard.gs");
	ResourceManager::getInstance()->GetShader("billboard").use().setInt("texture_billboard", 0);
	ResourceManager::getInstance()->LoadShader("particle_system", "asset/shaders/jcore/particle_system.vs", "asset/shaders/jcore/particle_system.fs", "asset/shaders/jcore/particle_system.gs");
	ResourceManager::getInstance()->GetShader("particle_system").use().setInt("texture_particle", 0);
	ResourceManager::getInstance()->LoadShader("post", "asset/shaders/jcore/post.vs", "asset/shaders/jcore/post.fs");
	ResourceManager::getInstance()->GetShader("post").use().setInt("scene", 0);
	ResourceManager::getInstance()->LoadShader("pick", "asset/shaders/jcore/pick.vs", "asset/shaders/jcore/pick.fs");
	ResourceManager::getInstance()->LoadShader("pick_animation", "asset/shaders/jcore/pick_animation.vs", "asset/shaders/jcore/pick_animation.fs");
	ResourceManager::getInstance()->LoadShader("shadow_map", "asset/shaders/jcore/shadow_map.vs", "asset/shaders/jcore/shadow_map.fs");
	//ResourceManager::getInstance()->LoadShader("terrain", "asset/shaders/jcore/terrain.vs", "asset/shaders/jcore/terrain.fs");
	//ResourceManager::getInstance()->GetShader("terrain").use().setInt("texture_grass", 0);
	//ResourceManager::getInstance()->GetShader("terrain").setInt("texture_rock", 1);
	//ResourceManager::getInstance()->GetShader("terrain").setInt("texture_snow", 2);
	ResourceManager::getInstance()->LoadShader("shadow_debug", "asset/shaders/jcore/shadow_debug.vs", "asset/shaders/jcore/shadow_debug.fs");
	ResourceManager::getInstance()->GetShader("shadow_debug").use().setInt("texture_shadow", 0);
	ResourceManager::getInstance()->LoadShader("csm", "asset/shaders/jcore/csm/csm.vs", "asset/shaders/jcore/csm/csm.fs");
	ResourceManager::getInstance()->LoadShader("csm_animation", "asset/shaders/jcore/csm/csm_animation.vs", "asset/shaders/jcore/csm/csm_animation.fs");
	//ResourceManager::getInstance()->LoadShader("csm_terrain", "asset/shaders/jcore/csm/csm_terrain.vs", "asset/shaders/jcore/csm/csm_terrain.fs");
	//ResourceManager::getInstance()->GetShader("csm_terrain").use().setInt("texture_grass", 0);
	//ResourceManager::getInstance()->GetShader("csm_terrain").setInt("texture_rock", 1);
	//ResourceManager::getInstance()->GetShader("csm_terrain").setInt("texture_snow", 2);
	//ResourceManager::getInstance()->GetShader("csm_terrain").setInt("texture_shadow[0]", 3);
	//ResourceManager::getInstance()->GetShader("csm_terrain").setInt("texture_shadow[1]", 4);
	//ResourceManager::getInstance()->GetShader("csm_terrain").setInt("texture_shadow[2]", 5);
	ResourceManager::getInstance()->LoadShader("csm_model", "asset/shaders/jcore/csm/csm_model.vs", "asset/shaders/jcore/csm/csm_model.fs");
	ResourceManager::getInstance()->GetShader("csm_model").use().setInt("texture_shadow[0]", 4);
	ResourceManager::getInstance()->GetShader("csm_model").setInt("texture_shadow[1]", 5);
	ResourceManager::getInstance()->GetShader("csm_model").setInt("texture_shadow[2]", 6);
	ResourceManager::getInstance()->LoadShader("csm_model_animation", "asset/shaders/jcore/csm/csm_model_animation.vs", "asset/shaders/jcore/csm/csm_model_animation.fs");
	ResourceManager::getInstance()->GetShader("csm_model_animation").use().setInt("texture_shadow[0]", 4);
	ResourceManager::getInstance()->GetShader("csm_model_animation").setInt("texture_shadow[1]", 5);
	ResourceManager::getInstance()->GetShader("csm_model_animation").setInt("texture_shadow[2]", 6);

	ResourceManager::getInstance()->LoadShader("terrain_upWater", "asset/shaders/jcore/terrain.vs", "asset/shaders/jcore/terrain_upWater.fs");
	ResourceManager::getInstance()->GetShader("terrain_upWater").use().setInt("texture_grass", 0);
	ResourceManager::getInstance()->GetShader("terrain_upWater").setInt("texture_rock", 1);
	ResourceManager::getInstance()->GetShader("terrain_upWater").setInt("texture_snow", 2);

	ResourceManager::getInstance()->LoadShader("terrain_underWater", "asset/shaders/jcore/terrain.vs", "asset/shaders/jcore/terrain_underWater.fs");
	ResourceManager::getInstance()->GetShader("terrain_underWater").use().setInt("texture_grass", 0);
	ResourceManager::getInstance()->GetShader("terrain_underWater").setInt("texture_rock", 1);
	ResourceManager::getInstance()->GetShader("terrain_underWater").setInt("texture_snow", 2);

	ResourceManager::getInstance()->LoadShader("terrain_underWater_caust", "asset/shaders/jcore/terrain.vs", "asset/shaders/jcore/terrain_underWater_caust.fs");
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").use().setInt("texture_grass", 0);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setInt("texture_rock", 1);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setInt("texture_snow", 2);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setInt("texture_caust", 3);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setFloat("caustScale", 4.0f);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setVec3("fogColor", glm::vec3(0.2f, 0.2f, 0.9f));
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setFloat("fogStart", 0.0f);
	ResourceManager::getInstance()->GetShader("terrain_underWater_caust").setFloat("fogEnd", 50.0f);

	ResourceManager::getInstance()->LoadShader("water", "asset/shaders/jcore/water/water.vs", "asset/shaders/jcore/water/water.fs");
	ResourceManager::getInstance()->GetShader("water").use().setInt("texture_reflect", 0);
	ResourceManager::getInstance()->GetShader("water").setInt("texture_normal", 1);
	ResourceManager::getInstance()->GetShader("water").setInt("texture_dudv", 2);
	ResourceManager::getInstance()->GetShader("water").setInt("texture_refract", 3);
	ResourceManager::getInstance()->GetShader("water").setInt("texture_depth", 4);
	ResourceManager::getInstance()->GetShader("water").setVec4("waterColor", glm::vec4(0.1f, 0.2f, 0.4f, 1.0f));
	ResourceManager::getInstance()->GetShader("water").setVec3("lightPos", glm::vec3(100, 100, 100));

	ResourceManager::getInstance()->LoadShader("csm_terrain_upWater", "asset/shaders/jcore/csm/csm_terrain.vs", "asset/shaders/jcore/csm/csm_terrain_upWater.fs");
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").use().setInt("texture_grass", 0);
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").setInt("texture_rock", 1);
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").setInt("texture_snow", 2);
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").setInt("texture_shadow[0]", 3);
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").setInt("texture_shadow[1]", 4);
	ResourceManager::getInstance()->GetShader("csm_terrain_upWater").setInt("texture_shadow[2]", 5);

	ResourceManager::getInstance()->LoadTexture("skybox", "asset/resources/skybox", "right.jpg", "left.jpg",
		"top.jpg", "bottom.jpg", "front.jpg","back.jpg");
	ResourceManager::getInstance()->LoadTexture("skybox2", "asset/resources/skybox2", "sp3right.jpg", "sp3left.jpg",
		"sp3top.jpg", "sp3bot.jpg", "sp3front.jpg", "sp3back.jpg");
	ResourceManager::getInstance()->LoadTexture("skybox3", "asset/resources/skybox3", "right.jpg", "left.jpg",
		"top.jpg", "bottom.jpg", "front.jpg", "back.jpg");

	//场景管理器
	m_Scene = new Scene();

	//场景根节点
	m_RootNode = new Node(_T("Scene_Root"));

	//相机节点
	m_CameraNode = AddNode(_T("MainCamera"));
	m_FreeCamera = new FreeCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FreeCamera->SetIsActive(true);
	m_FreeCamera->SetWorld(this);
	m_FollowCamera = new FollowCameraComponent(m_WindowWidth, m_WindowHeight);
	m_FollowCamera->SetIsActive(false);
	m_CameraNode->AddComponent(m_FreeCamera);
	m_CameraNode->AddComponent(m_FollowCamera);

	m_TextRender = new TextRenderer(m_WindowWidth, m_WindowHeight);
	m_TextRender->Load("asset/fonts/msyh.ttf", 36);

	//渲染器
	m_Renderer = new MainRenderer();
	m_Renderer->Initialize(m_WindowWidth, m_WindowHeight);
	return true;
}

void World::Update(double curFrame, double deltaFrame)
{
	m_CurrentFrame = curFrame;
	m_DeltaFrame = deltaFrame;
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
	RenderContext context;
	context.GetParamsFromCamera(GetActiveCamera());
	context.CurrentFrame = m_CurrentFrame;

	m_Renderer->Render(m_Scene, &context);

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
	
	Node* ret = 0;
	PickInfo pick = m_Renderer->Pick(m_Scene, &context, x, y);
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

	if (m_PickingNode != 0) {
		m_PickingNodeSRT = m_PickingNode->FindComponent<SRTTransformComponent>();
		if (m_PickingNodeSRT != 0) {
			glm::vec3 position = m_PickingNodeSRT->GetTranslation();
			glm::mat4 view = GetActiveCamera()->GetViewTransform();
			glm::mat4 proj = GetActiveCamera()->GetProjectionTransform();
			glm::mat4 vp = proj * view;
			glm::vec4 projCoord = vp * glm::vec4(position, 1.0f);
			glm::vec3 ndcCoord = projCoord / projCoord.w;
			m_PickNodeScreenZ = ndcCoord.z;
			m_PickingNodeSRT->SetCheckTerrain(false);
		}
		m_IsDragging = true;
	}
	else {
		m_PickingNodeSRT = 0;
	}

	return ret;
}

void World::OnMouseMove(double xPos, double yPos)
{
	if (m_IsDragging) {
		if (m_PickingNode != 0 && GetActiveCamera() == m_FreeCamera) {
			if (m_PickingNodeSRT != 0) {
				glm::mat4 view = GetActiveCamera()->GetViewTransform();
				glm::mat4 proj = GetActiveCamera()->GetProjectionTransform();
				glm::mat4 vp = proj * view;

				float x = xPos / (m_WindowWidth - 1.0) * 2.0f - 1.0f;
				float y = ((m_WindowHeight - 1.0f - yPos) / (m_WindowHeight - 1.0f)) * 2.0f - 1.0f;
				float z = m_PickNodeScreenZ;
				glm::vec4 pos = glm::inverse(vp) * glm::vec4(x, y, z, 1.0f);
				glm::vec3 worldPos = pos / pos.w;
				worldPos.y = __max(GetHeightAt(worldPos), worldPos.y);
				m_PickingNodeSRT->SetTranslation(worldPos);
			}
		}
	}
	else {
		GetActiveCamera()->OnMouseMove(xPos, yPos);
	}
}

void World::OnMouseDown()
{
	GetActiveCamera()->OnMouseDown();
}

void World::OnMouseUp()
{
	m_IsDragging = false;
	if (m_PickingNodeSRT != 0) {
		m_PickingNodeSRT->SetCheckTerrain(true);
	}
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
	m_WindowWidth = width;
	m_WindowHeight = height;
	m_FreeCamera->Resize(width, height);
	m_FollowCamera->Resize(width, height);

	m_Renderer->Resize(width, height);
	m_TextRender->Resize(width, height);
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

void World::ToFlyMode(bool value)
{
	m_FreeCamera->SetIsActive(true);
	m_FreeCamera->ToFlyMode(value);
	m_FollowCamera->SetIsActive(false);
}

bool& World::GetFlyMode()
{
	return m_FreeCamera->GetFlyMode();
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

CameraComponent* World::GetFreeCamera()
{
	return m_FreeCamera;
}

CameraComponent* World::GetFollowCamera()
{
	return m_FollowCamera;
}

Scene* World::GetScene()
{
	return m_Scene;
}

float World::GetHeightAt(const glm::vec3& position)
{
	if (m_Scene->GetTerrain() != 0) {
		return m_Scene->GetTerrain()->GetHeightAt(position);
	}

	return -FLT_MAX;
}

bool World::GetEnablePostEffect()
{
	return m_Renderer->GetEnablePostEffect();
}

void World::SetEnablePostEffect(bool value)
{
	m_Renderer->SetEnablePostEffect(value);
}

bool& World::GetEnableLight()
{
	return m_Renderer->GetEnableLighting();
}

void World::SetEnableLight(bool value)
{
	m_Renderer->SetEnableLighting(value);
}

bool& World::GetEnableSkybox()
{
	return m_Renderer->GetEnableSkybox();
}

void World::SetEnableSkybox(bool value)
{
	m_Renderer->SetEnableSkybox(value);
}

bool& World::GetEnableRenderNormal()
{
	return m_Renderer->GetEnableNormal();
}

void World::SetEnableRenderNormal(bool value)
{
	m_Renderer->SetEnableNormal(value);
}

int World::GetWindowWidth()
{
	return m_WindowWidth;
}

int World::GetWindowHeight()
{
	return m_WindowHeight;
}

float& World::GetFreeCamKeySensitivity()
{
	return m_FreeCamera->GetKeySensitivity();
}

glm::vec3& World::GetSunDirection()
{
	return m_Renderer->GetSunDirection();
}

void World::SetSunDirection(const glm::vec3& value)
{
	m_Renderer->SetSunDirection(value);
}

float& World::GetWaterHeight()
{
	return m_Renderer->GetWaterHeight();
}
