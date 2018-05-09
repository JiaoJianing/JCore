#include "stdafx.h"
#include "ResourceManager.h"
#include "World.h"
#include "CubeComponent.h"
#include "SRTTransformComponent.h"
#include "ModelComponent.h"
#include "SphereComponent.h"
#include "DirLightComponent.h"

float screenWidth = 800, screenHeight = 600;
float deltaFrame = 0.0f, lastFrame = 0.0f;
World world(screenWidth, screenHeight);
int nodeID = 0;

std::vector<Node*> nodes;
int nodeIndex = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
	world.OnMouseMove(xpos, ypos);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			world.OnMouseDown();

			double x, y;
			glfwGetCursorPos(window, &x, &y);
			Node* select = world.PickNode(x, screenHeight - y - 1);
		}
		else if (action == GLFW_RELEASE) {
			world.OnMouseUp();
		}
	}
}

void key_click_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) {
		return;
	}

	srand(glfwGetTime());

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_A:
	{
		Node* node = world.AddNode(_T("test") + std::to_wstring(nodeID++));
		node->SetColor(glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f));
		SRTTransformComponent* srt = new SRTTransformComponent();
		node->AddComponent(srt);
		CubeComponent* cube = new CubeComponent();
		node->AddComponent(cube);

		srt->SetTranslation(glm::vec3(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20));
		node->SetParent(world.FindNode(_T("child1")) ? world.FindNode(_T("child1")) : world.GetRootNode());
		std::wcout << _T("添加子节点: ") << node->GetName() << std::endl;
		
		nodes.push_back(node);
	}
		break;
	case GLFW_KEY_D:
	{
		//Node * node = scene.FindNode("parent1");
		//if (node) {
		//	scene.RemoveNode(node);
		//	std::cout << "删除 parent1 及其子节点" << std::endl;
		//}
	}
		break;
	case GLFW_KEY_1://切换free相机模式
	{
		world.ToFree(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	break;
	case GLFW_KEY_2://切换follow相机模式
	{
		world.ToFollow(nodes.at(nodeIndex++ % nodes.size()));
	}
	break;
	default:
		world.OnKeyboard(key);
		break;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		world.OnKeyboard(GLFW_KEY_UP);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		world.OnKeyboard(GLFW_KEY_DOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		world.OnKeyboard(GLFW_KEY_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		world.OnKeyboard(GLFW_KEY_RIGHT);
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	world.OnMouseScroll(xOffset, yOffset);
}

int main(int argc, char** argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "JCore", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);//鼠标移动
	glfwSetMouseButtonCallback(window, mouse_click_callback);//鼠标点击
	glfwSetKeyCallback(window, key_click_callback);//键盘按下
	glfwSetScrollCallback(window, scroll_callback);//鼠标滚轮

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);

	world.Initialize();
	world.SetEnablePostEffect(false);
	world.SetEnableLight(true);

	//地板
	Node* floor = world.AddNode(_T("floor"));
	floor->SetColor(glm::vec3(1.0f));
	floor->SetPickable(false);
	SRTTransformComponent* floorSRT = new SRTTransformComponent();
	floorSRT->SetScale(glm::vec3(20.0f, 0.1f, 20.0f));
	floorSRT->SetTranslation(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->AddComponent(floorSRT);
	CubeComponent* floorCube = new CubeComponent();
	floor->AddComponent(floorCube);

	//光源
	Node* dirLight = world.AddNode(_T("dirLight"));
	SRTTransformComponent* srtDirLight = new SRTTransformComponent();
	srtDirLight->SetTranslation(glm::vec3(1.0f, 5.0f, 0.0f));
	dirLight->AddComponent(srtDirLight);
	DirLightComponent* dirLightCmp = new DirLightComponent();
	dirLightCmp->SetAmbientIntensity(0.01f);
	dirLightCmp->SetDiffuseIntensity(1.0f);
	dirLight->AddComponent(dirLightCmp);

	//模型1
	Node* parent1 = world.AddNode(_T("parent1"));
	parent1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt1 = new SRTTransformComponent();
	srt1->SetTranslation(glm::vec3(2.0f, -1.0f, 0.0f));
	srt1->SetScale(glm::vec3(0.2f));
	parent1->AddComponent(srt1);
	ModelComponent* model1 = new ModelComponent("asset/models/nanosuit/nanosuit.obj");
	parent1->AddComponent(model1);

	//球
	Node* parent2 = world.AddNode(_T("parent2"));
	parent2->SetColor(glm::vec3(0.0f, 0.5f, 0.5f));
	SRTTransformComponent* srt2 = new SRTTransformComponent();
	srt2->SetTranslation(glm::vec3(-2.0f, 0.0f, 0.0f));
	parent2->AddComponent(srt2);
	SphereComponent* sphereCmp = new SphereComponent("asset/resources/bricks2.jpg", "asset/resources/bricks2_normal.jpg", "asset/resources/bricks2_specular.jpg");
	parent2->AddComponent(sphereCmp);

	//模型2
	Node* child1 = world.AddNode(_T("child1"));
	child1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	SRTTransformComponent* srt3 = new SRTTransformComponent();
	srt3->SetTranslation(glm::vec3(1.0f, 0.0f, 2.0f));
	child1->AddComponent(srt3);
	ModelComponent* model2 = new ModelComponent("asset/models/cyborg/cyborg.obj");
	child1->AddComponent(model2);
	//child1->SetParent(parent1);

	//立方体
	Node* child2 = world.AddNode(_T("child2"));
	SRTTransformComponent* srt4 = new SRTTransformComponent();
	srt4->SetTranslation(glm::vec3(-2.0f, 0.0f, 0.0f));
	//srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	child2->AddComponent(srt4);
	CubeComponent* cubeCmp4 = new CubeComponent("asset/resources/bricks2.jpg", "asset/resources/bricks2_normal.jpg", "asset/resources/bricks2_specular.jpg");
	child2->AddComponent(cubeCmp4);
	child2->SetParent(parent2);

	nodes.push_back(parent1);
	nodes.push_back(parent2);
	nodes.push_back(child1);
	nodes.push_back(child2);

	while (!glfwWindowShouldClose(window))
	{
		//绘制
		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		Node* n = world.FindNode(_T("child1"));
		if (n) {
			SRTTransformComponent* srt = 0;
			srt = n->FindComponent<SRTTransformComponent>();
			if (srt) {
				//srt->SetTranslation(glm::vec3(2.0f*sin(currentFrame), 0.0f, 2.0f*cos(currentFrame)));
				srt->SetRotate(glm::vec3(0, currentFrame * 50, 0));
			}
		}

		processInput(window);
		world.Update(currentFrame, deltaFrame);
		world.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::getInstance()->Clear();

	ResourceManager::getInstance()->deleteInstance();

	glfwTerminate();

	return 0;
}