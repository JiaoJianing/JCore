#include "stdafx.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "CubeComponent.h"
#include "SRTTransformComponent.h"
#include "ModelComponent.h"

float screenWidth = 800, screenHeight = 600;
float deltaFrame = 0.0f, lastFrame = 0.0f;
Scene scene(screenWidth, screenHeight);
int nodeID = 0;

std::vector<Node*> nodes;
int nodeIndex = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
	scene.OnMouseMove(xpos, ypos);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			scene.OnMouseDown();

			double x, y;
			glfwGetCursorPos(window, &x, &y);
			Node* select = scene.PickNode(x, screenHeight - y - 1);
		}
		else if (action == GLFW_RELEASE) {
			scene.OnMouseUp();
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
		Node* node = scene.AddNode(_T("test") + std::to_wstring(nodeID++));
		SRTTransformComponent* srt = new SRTTransformComponent();
		node->AddComponent(srt);
		CubeComponent* cube = new CubeComponent();
		cube->SetColor(glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f));
		node->AddComponent(cube);

		srt->SetTranslation(glm::vec3(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20));
		node->SetParent(scene.FindNode(_T("child1")) ? scene.FindNode(_T("child1")) : scene.GetRootNode());
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
		scene.ToFree(glm::vec3(0.0f, 0.0f, 3.0f));
	}
	break;
	case GLFW_KEY_2://切换follow相机模式
	{
		scene.ToFollow(nodes.at(nodeIndex++ % nodes.size()));
	}
	break;
	default:
		scene.OnKeyboard(key);
		break;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_UP);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_DOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_RIGHT);
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	scene.OnMouseScroll(xOffset, yOffset);
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
	glEnable(GL_CULL_FACE);

	scene.Initialize();
	Node* parent1 = scene.AddNode(_T("parent1"));
	SRTTransformComponent* srt1 = new SRTTransformComponent();
	parent1->AddComponent(srt1);
	//CubeComponent* cubeCmp1 = new CubeComponent();
	//cubeCmp1->SetColor(glm::vec3(0.5f, 0.5f, 0.0f));
	//parent1->AddComponent(cubeCmp1);
	ModelComponent* model1 = new ModelComponent("asset/models/nanosuit/nanosuit.obj");
	model1->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	parent1->AddComponent(model1);

	Node* parent2 = scene.AddNode(_T("parent2"));
	SRTTransformComponent* srt2 = new SRTTransformComponent();
	parent2->AddComponent(srt2);
	CubeComponent* cubeCmp2 = new CubeComponent();
	cubeCmp2->SetColor(glm::vec3(0.0f, 0.5f, 0.5f));
	cubeCmp2->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	parent2->AddComponent(cubeCmp2);

	Node* child1 = scene.AddNode(_T("child1"));
	SRTTransformComponent* srt3 = new SRTTransformComponent();
	child1->AddComponent(srt3);
	//CubeComponent* cubeCmp3 = new CubeComponent();
	//cubeCmp3->SetColor(glm::vec3(0.5f, 0.0f, 0.5f));
	//child1->AddComponent(cubeCmp3);
	ModelComponent* model2 = new ModelComponent("asset/models/cyborg/cyborg.obj");
	model2->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	child1->AddComponent(model2);
	child1->SetParent(parent1);

	Node* child2 = scene.AddNode(_T("child2"));
	SRTTransformComponent* srt4 = new SRTTransformComponent();
	child2->AddComponent(srt4);
	CubeComponent* cubeCmp4 = new CubeComponent();
	cubeCmp4->SetColor(glm::vec3(0.5f, 0.0f, 1.0f));
	cubeCmp4->SetHighLightColor(glm::vec3(1.0f, 0.0f, 1.0f));
	child2->AddComponent(cubeCmp4);
	child2->SetParent(parent2);

	srt1->SetTranslation(glm::vec3(2.0f, -1.0f, 0.0f));
	srt1->SetScale(glm::vec3(0.2f));
	srt2->SetTranslation(glm::vec3(-2.0f, 0.0f, 0.0f));

	srt3->SetTranslation(glm::vec3(2.0f, 0.0f, 0.0f));
	srt3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	srt4->SetTranslation(glm::vec3(-2.0f, 0.0f, 0.0f));
	srt4->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));

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

		Node* n = scene.FindNode(_T("child1"));
		if (n) {
			SRTTransformComponent* srt = 0;
			srt = n->FindComponent<SRTTransformComponent>();
			if (srt) {
				srt->SetTranslation(glm::vec3(2.0f*sin(currentFrame), 0.0f, 2.0f*cos(currentFrame)));
				//srt->SetRotate(glm::vec3(currentFrame * 50, currentFrame * 50, currentFrame * 50));
			}
		}

		processInput(window);
		scene.Update(currentFrame, deltaFrame);
		scene.Render();

		glfwSwapBuffers(window);
	}

	ResourceManager::getInstance()->Clear();

	ResourceManager::getInstance()->deleteInstance();

	glfwTerminate();

	return 0;
}