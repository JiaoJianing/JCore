#include "stdafx.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Camera.h"
#include "CubeComponent.h"

float screenWidth = 800, screenHeight = 600;
float deltaFrame = 0.0f, lastFrame = 0.0f;
Scene scene(screenWidth, screenHeight);
int nodeID = 0;

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
		Node* node = scene.AddNode("test" + std::to_string(nodeID++));
		CubeComponent* cube = new CubeComponent();
		cube->SetColor(glm::vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f));
		node->AddComponent(cube);

		node->SetTranslate(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20);
		node->SetParent(scene.FindNode("child1") ? scene.FindNode("child1") : scene.GetRootNode());
		std::cout << "添加子节点: " << node->GetName() << std::endl;
	}
		break;
	case GLFW_KEY_D:
	{
		Node * node = scene.FindNode("parent1");
		if (node) {
			scene.RemoveNode(node);
			std::cout << "删除 parent1 及其子节点" << std::endl;
		}
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
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	//	scene.OnKeyboard(GLFW_KEY_W);
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	//	scene.OnKeyboard(GLFW_KEY_S);
	//}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_DOWN);
	}
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	//	scene.OnKeyboard(GLFW_KEY_A);
	//}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		scene.OnKeyboard(GLFW_KEY_LEFT);
	}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	//	scene.OnKeyboard(GLFW_KEY_D);
	//}
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

	scene.Initialize();
	Node* parent1 = scene.AddNode("parent1");
	CubeComponent* cubeCmp1 = new CubeComponent();
	cubeCmp1->SetColor(glm::vec3(0.5f, 0.5f, 0.0f));
	parent1->AddComponent(cubeCmp1);

	Node* parent2 = scene.AddNode("parent2");
	CubeComponent* cubeCmp2 = new CubeComponent();
	cubeCmp2->SetColor(glm::vec3(0.0f, 0.5f, 0.5f));
	parent2->AddComponent(cubeCmp2);

	Node* child1 = scene.AddNode("child1");
	CubeComponent* cubeCmp3 = new CubeComponent();
	cubeCmp3->SetColor(glm::vec3(0.5f, 0.0f, 0.5f));
	child1->AddComponent(cubeCmp3);

	Node* child2 = scene.AddNode("child2");
	CubeComponent* cubeCmp4 = new CubeComponent();
	cubeCmp4->SetColor(glm::vec3(0.5f, 0.0f, 1.0f));
	child2->AddComponent(cubeCmp4);


	child1->SetParent(parent1);
	child2->SetParent(parent2);

	parent1->SetTranslate(2.0f, 0.0f, 0.0f);
	parent2->SetTranslate(-2.0f, 0.0f, 0.0f);

	child1->SetTranslate(2.0f, 0.0f, 0.0f);
	child1->SetScale(0.5f, 0.5f, 0.5f);
	child2->SetTranslate(-2.0f, 0.0f, 0.0f);
	child2->SetScale(0.5f, 1.0f, 0.5f);

	while (!glfwWindowShouldClose(window))
	{
		//绘制
		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Node* n = scene.FindNode("child1");
		if (n) {
			n->SetTranslate(2.0f*sin(currentFrame), 0.0f, 2.0f*cos(currentFrame));
			n->SetRotate(currentFrame * 50, currentFrame * 50, currentFrame * 50);
		}

		processInput(window);
		scene.Update(currentFrame, deltaFrame);
		scene.Render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ResourceManager::getInstance()->Clear();

	ResourceManager::getInstance()->deleteInstance();

	glfwTerminate();

	return 0;
}