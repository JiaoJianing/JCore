#include "stdafx.h"
#include "GlfwApplication.h"
#include "ResourceManager.h"

#pragma region GLFW回调
void glfw_onResize(GLFWwindow* window, int width, int height) {
	g_App->OnResize(width, height);
}

void glfw_onMouseMove(GLFWwindow* window, double x, double y) {
	g_App->OnMouseMove(x, y);
}

void glfw_OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
	g_App->OnMouseScroll(xOffset, yOffset);
}

void glfw_OnMouseClick(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		if (action == GLFW_PRESS) {
			g_App->OnLButtonDown(x, y);
		}
		else if (action == GLFW_RELEASE) {
			g_App->OnLButtonUp(x, y);
		}
	}
}

void glfw_OnKeyClick(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressed = (action == GLFW_PRESS);

	g_App->OnKeyboard(key, pressed);
}

void glfw_ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		g_App->OnKeyboard(GLFW_KEY_UP, true);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		g_App->OnKeyboard(GLFW_KEY_DOWN, true);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		g_App->OnKeyboard(GLFW_KEY_LEFT, true);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		g_App->OnKeyboard(GLFW_KEY_RIGHT, true);
	}
}

#pragma endregion

GlfwApplication::GlfwApplication()
	: m_GlfwWindow(0)
{
}


GlfwApplication::~GlfwApplication()
{
}

bool GlfwApplication::Initialize(int windowWidth, int windowHeight, const std::string& appName)
{
	bool result = __super::Initialize(windowWidth, windowHeight, appName);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_GlfwWindow = glfwCreateWindow(GetWindowWidth(), GetWindowHeight(), GetAppName().c_str(), 0, 0);
	if (m_GlfwWindow == 0) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		result = false;
	}
	glfwMakeContextCurrent(m_GlfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		result = false;
	}

	glfwSetFramebufferSizeCallback(m_GlfwWindow, glfw_onResize);
	glfwSetCursorPosCallback(m_GlfwWindow, glfw_onMouseMove);//鼠标移动
	glfwSetMouseButtonCallback(m_GlfwWindow, glfw_OnMouseClick);//鼠标点击
	glfwSetKeyCallback(m_GlfwWindow, glfw_OnKeyClick);//键盘按下
	glfwSetScrollCallback(m_GlfwWindow, glfw_OnMouseScroll);//鼠标滚轮

	glViewport(0, 0, 800, 600);

	//初始化World并执行回调
	if (!GetWorld()->Initialize()) {
		std::cout << "Failed to initialize World in GLFWApplication!" << std::endl;
		result = false;
	}

	if (result && OnInitializeCallback != 0) {
		OnInitializeCallback(GetWorld());
	}

	return result;
}

void GlfwApplication::Run()
{
	while (!glfwWindowShouldClose(m_GlfwWindow))
	{
		float currentFrame = glfwGetTime();
		m_DeltaFrame = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
		glfwPollEvents();

		glfw_ProcessInput(m_GlfwWindow);

		//执行更新回调
		if (OnUpdateCallback != 0) {
			OnUpdateCallback(GetWorld(), currentFrame, m_DeltaFrame);
		}

		GetWorld()->Update(currentFrame, m_DeltaFrame);
		GetWorld()->Render();

		glfwSwapBuffers(m_GlfwWindow);
	}

	ResourceManager::getInstance()->Clear();

	ResourceManager::getInstance()->deleteInstance();

	glfwTerminate();
}

void GlfwApplication::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	SetWindowWidth(width);
	SetWindowHeight(height);
	GetWorld()->OnResize(width, height);
}

void GlfwApplication::OnMouseMove(double x, double y)
{
	GetWorld()->OnMouseMove(x, y);
}

void GlfwApplication::OnMouseScroll(double xOffset, double yOffset)
{
	GetWorld()->OnMouseScroll(xOffset, yOffset);
}

void GlfwApplication::OnLButtonDown(double x, double y)
{
	GetWorld()->OnMouseDown();
	Node* select = GetWorld()->PickNode(x, GetWindowHeight() - y - 1);
}

void GlfwApplication::OnLButtonUp(double x, double y)
{
	GetWorld()->OnMouseUp();
}

void GlfwApplication::OnKeyboard(int key, bool pressed)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(m_GlfwWindow, true);
		break;
	default:
		if (OnKeyboardCallback != 0) {
			OnKeyboardCallback(GetWorld(), key, pressed);
		}
		GetWorld()->OnKeyboard(key);
		break;
	}
}
