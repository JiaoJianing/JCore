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
#include "BillboardComponent.h"
#include "SnowParticleSystemComponent.h"
#include "TerrainComponent.h"
#include "AntTweakBar.h"
#include "StringHelper.h"
#include <random>

int nodeID = 0;

std::vector<Node*> nodes;
int nodeIndex = 0;
Node* multipleAnimNode = 0;
int animIndex = 0;

TwBar* g_TwBar = 0;

glm::vec3 getRandomPosition(World* world) {
	std::random_device rd;

	float x = -200.0f + rd() % 400;
	float y = 0;
	float z = -200.0f + rd() % 400;
	float height = world->GetHeightAt(glm::vec3(x, y, z));
	return glm::vec3(x, height, z);
}

Node* addAnimationModel(World* world, stringT nodeName, const char* path, glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, float heightAboveTerrain=0.0f) {
	Node* animationBody = world->AddNode(nodeName);
	animationBody->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* animationSrt = new SRTTransformComponent();
	animationSrt->SetTranslation(getRandomPosition(world));
	animationSrt->SetRotate(rotate);
	animationSrt->SetScale(scale);
	animationSrt->SetHeightAboveTerrain(heightAboveTerrain);
	animationBody->AddComponent(animationSrt);
	ModelComponent* animationModel = new ModelComponent(path);
	animationBody->AddComponent(animationModel);

	nodes.push_back(animationBody);
	return animationBody;
}

void OnWorldInit(World* world) {
	world->SetEnablePostEffect(true);
	world->SetEnableLight(true);
	world->SetEnableSkybox(true);
	world->SetEnableRenderNormal(false);
	world->ToFlyMode(true);

	//地形
	Node* terrain = world->AddNode(_T("terrain"));
	terrain->SetPickable(false);
	SRTTransformComponent* terrainSRT = new SRTTransformComponent();
	terrainSRT->SetCheckTerrain(false);
	terrain->AddComponent(terrainSRT);
	TerrainComponent* terrainCmp = new TerrainComponent("asset/resources/Terrain/terrain0-16bbp-257x257.raw", 16, 257, 257, 30.0f, 2.0f);
	terrain->AddComponent(terrainCmp);

	//地板
	//Node* floor = world->AddNode(_T("floor"));
	//floor->SetColor(glm::vec3(1.0f));
	//floor->SetPickable(false);
	//SRTTransformComponent* floorSRT = new SRTTransformComponent();
	//floorSRT->SetScale(glm::vec3(20.0f, 0.1f, 20.0f));
	//floorSRT->SetTranslation(glm::vec3(0.0f, 26.0f, 0.0f));
	//floor->AddComponent(floorSRT);
	//CubeComponent* floorCube = new CubeComponent("asset/resources/bricks2.jpg", "asset/resources/bricks2_normal.jpg", "asset/resources/bricks2_specular.jpg");
	//floor->AddComponent(floorCube);

	//方向光
	//Node* dirLight = world->AddNode(_T("dirLight"));
	//SRTTransformComponent* srtDirLight = new SRTTransformComponent();
	//srtDirLight->SetTranslation(glm::vec3(.0f, 28.0f, 1.0f));
	//dirLight->AddComponent(srtDirLight);
	//DirLightComponent* dirLightCmp = new DirLightComponent();
	//dirLightCmp->SetAmbientIntensity(0.05f);
	//dirLightCmp->SetDiffuseIntensity(0.2f);
	//dirLight->AddComponent(dirLightCmp);
	//BillboardComponent* sunBillboard = new BillboardComponent("asset/resources/sunny.png");
	//dirLight->AddComponent(sunBillboard);

	//点光源1
	Node* pointLight1 = world->AddNode(_T("pointLight1"));
	SRTTransformComponent* srtPointLight1 = new SRTTransformComponent();
	srtPointLight1->SetCheckTerrain(false);
	srtPointLight1->SetTranslation(glm::vec3(-5.0f, 32.0f, -5.0f));
	pointLight1->AddComponent(srtPointLight1);
	PointLightComponent* pointLightCmp1 = new PointLightComponent();
	pointLightCmp1->SetLightColor(glm::vec3(1.0f, 1.0f, 0.0f));
	pointLightCmp1->SetAmbientIntensity(0.05f);
	pointLightCmp1->SetDiffuseIntensity(0.5f);
	pointLight1->AddComponent(pointLightCmp1);
	//点光源2
	Node* pointLight2 = world->AddNode(_T("pointLight2"));
	SRTTransformComponent* srtPointLight2 = new SRTTransformComponent();
	srtPointLight2->SetCheckTerrain(false);
	srtPointLight2->SetTranslation(glm::vec3(-2.0f, 32.0f, 3.0f));
	pointLight2->AddComponent(srtPointLight2);
	PointLightComponent* pointLightCmp2 = new PointLightComponent();
	pointLightCmp2->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	pointLightCmp2->SetAmbientIntensity(0.05f);
	pointLightCmp2->SetDiffuseIntensity(0.6f);
	pointLight2->AddComponent(pointLightCmp2);
	//聚光灯1
	Node* spotLight1 = world->AddNode(_T("spotLight1"));
	SRTTransformComponent* srtSpotLight1 = new SRTTransformComponent();
	srtSpotLight1->SetCheckTerrain(false);
	srtSpotLight1->SetTranslation(glm::vec3(-2.0f, 32.0f, -3.0f));
	spotLight1->AddComponent(srtSpotLight1);
	SpotLightComponent* spotLightCmp1 = new SpotLightComponent();
	spotLightCmp1->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	spotLightCmp1->SetAmbientIntensity(0.05f);
	spotLightCmp1->SetDiffuseIntensity(0.6f);
	spotLightCmp1->SetCutOff(20.0f);
	spotLightCmp1->SetDirection(glm::vec3(2.0, -5.0, -3.0));
	spotLight1->AddComponent(spotLightCmp1);
	//聚光灯2
	Node* spotLight2 = world->AddNode(_T("spotLight2"));
	SRTTransformComponent* srtSpotLight2 = new SRTTransformComponent();
	srtSpotLight2->SetCheckTerrain(false);
	srtSpotLight2->SetTranslation(glm::vec3(5.0f, 32.0f, 0.0f));
	spotLight2->AddComponent(srtSpotLight2);
	SpotLightComponent* spotLightCmp2 = new SpotLightComponent();
	spotLightCmp2->SetLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	spotLightCmp2->SetAmbientIntensity(0.05f);
	spotLightCmp2->SetDiffuseIntensity(1.0f);
	spotLightCmp2->SetCutOff(60.0f);
	spotLightCmp2->SetDirection(glm::vec3(-5.0f, -5.0f, 0.0));
	spotLight2->AddComponent(spotLightCmp2);

	Node* n1 = addAnimationModel(world, _T("purpleheartanime"), "asset/animate_models/purpleheartanime/purpleheartanime.fbx", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(-4.0f, 27.0f, 8.0f));
	multipleAnimNode = n1;
	//Node* n3 = addAnimationModel(world, _T("aatrox"), "asset/animate_models/aatrox/aatrox.dae", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(-1.0f, 26.0f, 4.0f));
	//Node* n4 = addAnimationModel(world, _T("Borvar"), "asset/animate_models/Borvar/Borvar.dae", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(2.0f, 26.0f, 4.0f));
	//Node* n5 = addAnimationModel(world, _T("cartoon_turtle"), "asset/animate_models/cartoon_turtle/cartoon_turtle.dae", glm::vec3(4.0f), glm::vec3(0.0f), glm::vec3(5.0f, 26.0f, 4.0f));
	//n5->SetFrontDir(glm::vec3(1.0f, 0.0f, 0.0f));
	//Node* n6 = addAnimationModel(world, _T("crazy_frog"), "asset/animate_models/crazy_frog/crazy_frog.dae", glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(8.0f, 26.0f, 4.0f));
	//Node* n7 = addAnimationModel(world, _T("dancing_crab"), "asset/animate_models/dancing_crab/dancing_crab.dae", glm::vec3(40.0f), glm::vec3(0.0f), glm::vec3(8.0f, 28.0f, 0.0f), 1.5f);
	//n7->SetFrontDir(glm::vec3(0.0f, 0.0f, -1.0f));
	//Node* n8 = addAnimationModel(world, _T("dragon"), "asset/animate_models/dragon/dragon.dae", glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(5.0f, 26.0f, 0.0f));
	//Node* n9 = addAnimationModel(world, _T("gex-4-walk"), "asset/animate_models/gex-4-walk/gex-4-walk.dae", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(2.0f, 26.0f, 0.0f));
	//Node* n10 = addAnimationModel(world, _T("gregory-dragon-ball"), "asset/animate_models/gregory-dragon-ball/gregory-dragon-ball.dae", glm::vec3(3.0f), glm::vec3(0.0f), glm::vec3(-1.0f, 26.0f, 0.0f));
	//Node* n11 = addAnimationModel(world, _T("lambert"), "asset/animate_models/lambert/lambert.dae", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(-4.0f, 26.0f, 0.0f));
	//Node* n13 = addAnimationModel(world, _T("ava-yonng"), "asset/animate_models/ava-yonng/ava-yonng.dae", glm::vec3(40.0f), glm::vec3(0.0f), glm::vec3(-7.0f, 26.0f, 8.0f));
	//Node* n14 = addAnimationModel(world, _T("bristleback"), "asset/animate_models/bristleback/bristleback.dae", glm::vec3(1.5f), glm::vec3(0.0f), glm::vec3(-4.0f, 26.0f, 8.0f));
	//Node* n15 = addAnimationModel(world, _T("greet_frog"), "asset/animate_models/greet_frog/greet_frog.dae", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(-1.0f, 26.0f, 8.0f));
	//Node* n16 = addAnimationModel(world, _T("phoenix-bird"), "asset/animate_models/phoenix-bird/phoenix-bird.dae", glm::vec3(3.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(2.0f, 27.0f, 8.0f));
	//n16->SetFrontDir(glm::vec3(1.0f, 0.0f, 0.0f));

	//模型1
	Node* parent1 = world->AddNode(_T("parent1"));
	parent1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt1 = new SRTTransformComponent();
	srt1->SetTranslation(glm::vec3(2.0f, 26.0f, -3.0f));
	srt1->SetHeightAboveTerrain(1);
	srt1->SetScale(glm::vec3(0.2f));
	parent1->AddComponent(srt1);
	ModelComponent* model1 = new ModelComponent("asset/models/nanosuit/nanosuit.obj");
	parent1->AddComponent(model1);

	//模型2
	Node* child1 = world->AddNode(_T("child1"));
	child1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt3 = new SRTTransformComponent();
	srt3->SetTranslation(glm::vec3(1.0f, 27.0f, -5.0f));
	srt3->SetHeightAboveTerrain(1);
	child1->AddComponent(srt3);
	ModelComponent* model2 = new ModelComponent("asset/models/cyborg/cyborg.obj");
	child1->AddComponent(model2);
	//child1->SetParent(parent1);

	//立方体
	Node* child2 = world->AddNode(_T("child2"));
	SRTTransformComponent* srt4 = new SRTTransformComponent();
	srt4->SetTranslation(glm::vec3(-5.0f, 27.0f, -5.0f));
	srt4->SetHeightAboveTerrain(2);
	//srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	child2->AddComponent(srt4);
	CubeComponent* cubeCmp4 = new CubeComponent("asset/resources/toy_box_diffuse.png", "asset/resources/toy_box_normal.png", "asset/resources/toy_box_specular.png");
	child2->AddComponent(cubeCmp4);

	//球体
	Node* sphere = world->AddNode(_T("sphere"));
	SRTTransformComponent* sphereSrt = new SRTTransformComponent();
	sphereSrt->SetTranslation(glm::vec3(-2.0f, 27.0f, -5.0f));
	sphereSrt->SetHeightAboveTerrain(2);
	//srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	sphere->AddComponent(sphereSrt);
	SphereComponent* sphereCmp = new SphereComponent("asset/resources/toy_box_diffuse.png", "asset/resources/toy_box_normal.png", "asset/resources/toy_box_specular.png");
	sphere->AddComponent(sphereCmp);

	//粒子系统 雪
	Node* snow = world->AddNode(_T("snow"));
	SnowParticleSystemComponent* snowCmp = new SnowParticleSystemComponent();
	snowCmp->Initialize(10000);
	snow->AddComponent(snowCmp);

	g_TwBar = TwNewBar("JCore");
	float refresh = 0.1f;
	TwSetParam(g_TwBar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);

	TwDefine("JCore/GLOBAL_TOOGLE label='Global Toggle'");
	
	TwWindowSize(world->GetWindowWidth(), world->GetWindowHeight());
	TwAddVarRW(g_TwBar, "normal", TW_TYPE_BOOLCPP, &world->GetEnableRenderNormal(), "help='Turn On/Off normal' group='GLOBAL_TOOGLE'");
	TwAddVarRW(g_TwBar, "fly", TW_TYPE_BOOLCPP, &world->GetFlyMode(), "help='Turn On/Off fly mode' group='GLOBAL_TOOGLE'");
	TwAddVarRW(g_TwBar, "light", TW_TYPE_BOOLCPP, &world->GetEnableLight(), "help='Turn On/Off light' group='GLOBAL_TOOGLE'");
	TwAddVarRW(g_TwBar, "skybox", TW_TYPE_BOOLCPP, &world->GetEnableSkybox(), "help='Turn On/Off skybox' group='GLOBAL_TOOGLE'");
	TwAddSeparator(g_TwBar, "", "");

	TwAddButton(g_TwBar, "camera-mode", 0, 0, "label='Current Camera: free'");
	TwAddButton(g_TwBar, "pick-info", 0, 0, "label='Current Pick: none'");

	TwStructMember vec3Members[] = {
		{"x", TW_TYPE_FLOAT, offsetof(glm::vec3, x), "" },
		{"y", TW_TYPE_FLOAT, offsetof(glm::vec3, y), "" },
		{"z", TW_TYPE_FLOAT, offsetof(glm::vec3, z), "" }
	};
	TwType typeVec3 = TwDefineStruct("glm::vec3", vec3Members, 3, sizeof(glm::vec3), 0, 0);
	TwAddButton(g_TwBar, "FPS Camera", 0, 0, "");
	TwAddVarRO(g_TwBar, "position", typeVec3, (void*)&world->GetFreeCamera()->GetPos(), 0);
	TwAddVarRW(g_TwBar, "direction", TW_TYPE_DIR3F, (void*)&world->GetFreeCamera()->GetTarget(), "");
	TwAddVarRW(g_TwBar, "sun_dir", TW_TYPE_DIR3F, (void*)&world->GetSunDirection(), "");
	TwAddVarRW(g_TwBar, "key-sensitivity", TW_TYPE_FLOAT, &world->GetFreeCamKeySensitivity(), "min=1 max=50 step=1");
	TwAddVarRW(g_TwBar, "water-height", TW_TYPE_FLOAT, &world->GetWaterHeight(), "min=1 max=20 step=1");
	TwAddSeparator(g_TwBar, "", "");
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

	//Node* sun = world->FindNode(_T("dirLight"));
	//if (sun) {
	//	SRTTransformComponent* srt = 0;
	//	srt = sun->FindComponent<SRTTransformComponent>();
	//	if (srt) {
	//		srt->SetTranslation(glm::vec3(20.0f*sin(currentFrame*0.01), 37.0f, 20.0f*cos(currentFrame*0.01)));
	//	}
	//}
}

void OnWorldKeyboard(World* world, int key, bool pressed) {
	if (!pressed) {
		return;
	}

	srand(glfwGetTime());

	switch (key)
	{
	case GLFW_KEY_1://切换free相机模式
	{
		world->ToFree(glm::vec3(0.0f, 30.0f, 3.0f));

		TwSetParam(g_TwBar, "camera-mode", "label", TW_PARAM_CSTRING, 1, "Current Camera: free");
	}
	break;
	case GLFW_KEY_2://切换follow相机模式
	{
		Node* node = nodes.at(nodeIndex++ % nodes.size());
		world->ToFollow(node);

		char paramValue[64];
		_snprintf(paramValue, sizeof(paramValue), "Camera fix to: %s", StringHelper::WString2String(node->GetName()).c_str());
		TwSetParam(g_TwBar, "camera-mode", "label", TW_PARAM_CSTRING, 1, paramValue);
	}
	break; 
	case GLFW_KEY_3:
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
	case GLFW_KEY_4:
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
	case GLFW_KEY_5:
	{
		if (multipleAnimNode != 0) {
			multipleAnimNode->SetCurrentAnimation(animIndex++ % 6);
		}
	}
	break;
	default:
		break;
	}
}

void OnWorldMouseDown(World* world, double x, double y) {

	Node* select = world->PickNode(x, world->GetWindowHeight() - y - 1);

	if (select != 0) {
		char paramValue[64];
		_snprintf(paramValue, sizeof(paramValue), "Current Pick: %s", StringHelper::WString2String(select->GetName()).c_str());
		TwSetParam(g_TwBar, "pick-info", "label", TW_PARAM_CSTRING, 1, paramValue);
	}
	else {
		TwSetParam(g_TwBar, "pick-info", "label", TW_PARAM_CSTRING, 1, "Current Pick: none");
	}
}

int main(int argc, char** argv) {

	g_App = new GlfwApplication();

	g_App->OnInitializeCallback = OnWorldInit;
	g_App->OnUpdateCallback = OnWorldUpdate;
	g_App->OnKeyboardCallback = OnWorldKeyboard;
	g_App->OnMouseDownCallback = OnWorldMouseDown;

	if (!g_App->Initialize(800, 600, "JCore")) {
		std::cout << "Failed to initialize application" << std::endl;
		return -1;
	}

	g_App->Run();

	delete g_App;

	return 0;
}