#include "stdafx.h"
#include "ResourceManager.h"
#include "World.h"
#include "CubeComponent.h"
#include "SRTTransformComponent.h"
#include "ModelComponent.h"
#include "DirLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "GlfwApplication.h"
#include "SphereComponent.h"

int nodeID = 0;

std::vector<Node*> nodes;
int nodeIndex = 0;

void OnWorldInit(World* world) {
	world->SetEnablePostEffect(true);
	world->SetEnableLight(true);
	world->SetEnableSkybox(true);
	world->SetEnableRenderNormal(true);

	//地板
	Node* floor = world->AddNode(_T("floor"));
	floor->SetColor(glm::vec3(1.0f));
	floor->SetPickable(false);
	SRTTransformComponent* floorSRT = new SRTTransformComponent();
	floorSRT->SetScale(glm::vec3(20.0f, 0.1f, 20.0f));
	floorSRT->SetTranslation(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->AddComponent(floorSRT);
	CubeComponent* floorCube = new CubeComponent("asset/resources/bricks2.jpg", "asset/resources/bricks2_normal.jpg", "asset/resources/bricks2_specular.jpg");
	floor->AddComponent(floorCube);

	//方向光
	Node* dirLight = world->AddNode(_T("dirLight"));
	SRTTransformComponent* srtDirLight = new SRTTransformComponent();
	srtDirLight->SetTranslation(glm::vec3(1.0f, 1.0f, 1.0f));
	dirLight->AddComponent(srtDirLight);
	DirLightComponent* dirLightCmp = new DirLightComponent();
	dirLightCmp->SetAmbientIntensity(0.05f);
	dirLightCmp->SetDiffuseIntensity(0.2f);
	dirLight->AddComponent(dirLightCmp);
	//点光源1
	Node* pointLight1 = world->AddNode(_T("pointLight1"));
	SRTTransformComponent* srtPointLight1 = new SRTTransformComponent();
	srtPointLight1->SetTranslation(glm::vec3(-5.0f, 5.0f, -5.0f));
	pointLight1->AddComponent(srtPointLight1);
	PointLightComponent* pointLightCmp1 = new PointLightComponent();
	pointLightCmp1->SetLightColor(glm::vec3(1.0f, 1.0f, 0.0f));
	pointLightCmp1->SetAmbientIntensity(0.05f);
	pointLightCmp1->SetDiffuseIntensity(0.5f);
	pointLight1->AddComponent(pointLightCmp1);
	//点光源2
	Node* pointLight2 = world->AddNode(_T("pointLight2"));
	SRTTransformComponent* srtPointLight2 = new SRTTransformComponent();
	srtPointLight2->SetTranslation(glm::vec3(-2.0f, 5.0f, 3.0f));
	pointLight2->AddComponent(srtPointLight2);
	PointLightComponent* pointLightCmp2 = new PointLightComponent();
	pointLightCmp2->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	pointLightCmp2->SetAmbientIntensity(0.05f);
	pointLightCmp2->SetDiffuseIntensity(0.6f);
	pointLight2->AddComponent(pointLightCmp2);
	//聚光灯1
	Node* spotLight1 = world->AddNode(_T("spotLight1"));
	SRTTransformComponent* srtSpotLight1 = new SRTTransformComponent();
	srtSpotLight1->SetTranslation(glm::vec3(-2.0f, 5.0f, 3.0f));
	spotLight1->AddComponent(srtSpotLight1);
	SpotLightComponent* spotLightCmp1 = new SpotLightComponent();
	//spotLightCmp1->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	spotLightCmp1->SetAmbientIntensity(0.05f);
	spotLightCmp1->SetDiffuseIntensity(0.6f);
	spotLightCmp1->SetCutOff(20.0f);
	spotLightCmp1->SetDirection(glm::vec3(2.0, -5.0, -3.0));
	spotLight1->AddComponent(spotLightCmp1);
	//聚光灯2
	Node* spotLight2 = world->AddNode(_T("spotLight2"));
	SRTTransformComponent* srtSpotLight2 = new SRTTransformComponent();
	srtSpotLight2->SetTranslation(glm::vec3(5.0f, 5.0f, 0.0f));
	spotLight2->AddComponent(srtSpotLight2);
	SpotLightComponent* spotLightCmp2 = new SpotLightComponent();
	//spotLightCmp2->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	spotLightCmp2->SetAmbientIntensity(0.05f);
	spotLightCmp2->SetDiffuseIntensity(1.0f);
	spotLightCmp2->SetCutOff(60.0f);
	spotLightCmp2->SetDirection(glm::vec3(-5.0f, -5.0f, 0.0));
	spotLight2->AddComponent(spotLightCmp2);

	//模型1
	Node* parent1 = world->AddNode(_T("parent1"));
	parent1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt1 = new SRTTransformComponent();
	srt1->SetTranslation(glm::vec3(2.0f, -1.0f, 0.0f));
	srt1->SetScale(glm::vec3(0.2f));
	parent1->AddComponent(srt1);
	ModelComponent* model1 = new ModelComponent("asset/models/nanosuit/nanosuit.obj");
	parent1->AddComponent(model1);

	//模型2
	Node* child1 = world->AddNode(_T("child1"));
	child1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt3 = new SRTTransformComponent();
	srt3->SetTranslation(glm::vec3(1.0f, 0.0f, 2.0f));
	child1->AddComponent(srt3);
	ModelComponent* model2 = new ModelComponent("asset/models/cyborg/cyborg.obj");
	child1->AddComponent(model2);
	//child1->SetParent(parent1);

	//立方体
	Node* child2 = world->AddNode(_T("child2"));
	SRTTransformComponent* srt4 = new SRTTransformComponent();
	srt4->SetTranslation(glm::vec3(-5.0f, 0.0f, 0.0f));
	//srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	child2->AddComponent(srt4);
	CubeComponent* cubeCmp4 = new CubeComponent("asset/resources/toy_box_diffuse.png", "asset/resources/toy_box_normal.png", "asset/resources/toy_box_specular.png");
	child2->AddComponent(cubeCmp4);

	//球体
	Node* sphere = world->AddNode(_T("sphere"));
	SRTTransformComponent* sphereSrt = new SRTTransformComponent();
	sphereSrt->SetTranslation(glm::vec3(-2.0f, 0.0f, 0.0f));
	//srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	sphere->AddComponent(sphereSrt);
	SphereComponent* sphereCmp = new SphereComponent("asset/resources/toy_box_diffuse.png", "asset/resources/toy_box_normal.png", "asset/resources/toy_box_specular.png");
	sphere->AddComponent(sphereCmp);

	nodes.push_back(dirLight);
	nodes.push_back(pointLight1);
	nodes.push_back(pointLight2);
	nodes.push_back(spotLight1);
	nodes.push_back(spotLight2);
	nodes.push_back(parent1);
	nodes.push_back(child1);
	nodes.push_back(child2);
}

void OnWorldUpdate(World* world, float currentFrame, float deltaFrame) {
	Node* n = world->FindNode(_T("child1"));
	if (n) {
		SRTTransformComponent* srt = 0;
		srt = n->FindComponent<SRTTransformComponent>();
		if (srt) {
			//srt->SetTranslation(glm::vec3(2.0f*sin(currentFrame), 0.0f, 2.0f*cos(currentFrame)));
			srt->SetRotate(glm::vec3(0, currentFrame * 50, 0));
		}
	}
}

void OnWorldKeyboard(World* world, int key, bool pressed) {
	if (!pressed) {
		return;
	}

	srand(glfwGetTime());

	switch (key)
	{

	case GLFW_KEY_A:
	{
		Node* node = world->AddNode(_T("test") + std::to_wstring(nodeID++));
		node->SetColor(glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f));
		SRTTransformComponent* srt = new SRTTransformComponent();
		node->AddComponent(srt);
		CubeComponent* cube = new CubeComponent();
		node->AddComponent(cube);

		srt->SetTranslation(glm::vec3(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20));
		node->SetParent(world->FindNode(_T("child1")) ? world->FindNode(_T("child1")) : world->GetRootNode());
		std::wcout << _T("添加子节点: ") << node->GetName() << std::endl;

		nodes.push_back(node);
	}
	break;
	case GLFW_KEY_D:
	{
		if (nodes.size() > 0) {
			Node * node = nodes[0];
			if (node) {
				std::cout << "删除节点: ";
				std::wcout << node->GetName() << std::endl;
				world->RemoveNode(node);
			}
			nodes.erase(nodes.begin());
		}
	}
	break;
	case GLFW_KEY_1://切换free相机模式
	{
		world->ToFree(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	break;
	case GLFW_KEY_2://切换follow相机模式
	{
		world->ToFollow(nodes.at(nodeIndex++ % nodes.size()));
	}
	break;
	default:
		break;
	}
}

int main(int argc, char** argv) {

	g_App = new GlfwApplication();

	g_App->OnInitializeCallback = OnWorldInit;
	g_App->OnUpdateCallback = OnWorldUpdate;
	g_App->OnKeyboardCallback = OnWorldKeyboard;

	if (!g_App->Initialize(800, 600, "JCore")) {
		std::cout << "Failed to initialize application" << std::endl;
		return -1;
	}

	g_App->Run();

	delete g_App;

	return 0;
}