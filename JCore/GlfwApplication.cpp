#include "stdafx.h"
#include "GlfwApplication.h"
#include "ResourceManager.h"
#include "AntTweakBar.h"

#pragma region GLFW回调

int glfwKey2TwKey(int glfwKey) {
	switch (glfwKey) {
	case GLFW_KEY_BACKSPACE:
		return TW_KEY_BACKSPACE;
	case GLFW_KEY_TAB:
		return TW_KEY_TAB;
	case GLFW_KEY_ENTER:
		return TW_KEY_RETURN;
	case GLFW_KEY_ESCAPE:
		return TW_KEY_ESCAPE;
	case GLFW_KEY_DELETE:
		return TW_KEY_DELETE;
	case GLFW_KEY_UP:
		return TW_KEY_UP;
	case GLFW_KEY_DOWN:
		return TW_KEY_DOWN;
	case GLFW_KEY_RIGHT:
		return TW_KEY_RIGHT;
	case GLFW_KEY_LEFT:
		return TW_KEY_LEFT;
	case GLFW_KEY_INSERT:
		return TW_KEY_INSERT;
	case GLFW_KEY_HOME:
		return TW_KEY_HOME;
	case GLFW_KEY_END:
		return TW_KEY_END;
	case GLFW_KEY_PAGE_UP:
		return TW_KEY_PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:
		return TW_KEY_PAGE_DOWN;
	case GLFW_KEY_F1:
		return TW_KEY_F1;
	case GLFW_KEY_F2:
		return TW_KEY_F2;
	case GLFW_KEY_F3:
		return TW_KEY_F3;
	case GLFW_KEY_F4:
		return TW_KEY_F4;
	case GLFW_KEY_F5:
		return TW_KEY_F5;
	case GLFW_KEY_F6:
		return TW_KEY_F6;
	case GLFW_KEY_F7:
		return TW_KEY_F7;
	case GLFW_KEY_F8:
		return TW_KEY_F8;
	case GLFW_KEY_F9:
		return TW_KEY_F9;
	case GLFW_KEY_F10:
		return TW_KEY_F10;
	case GLFW_KEY_F11:
		return TW_KEY_F11;
	case GLFW_KEY_F12:
		return TW_KEY_F12;
	default:
		std::cout << "UnImplemented Key: " << glfwKey << std::endl;
		break;
	}

	return -1;
}

void glfw_onResize(GLFWwindow* window, int width, int height) {
	g_App->OnResize(width, height);

	TwWindowSize(width, height);
}

void glfw_onMouseMove(GLFWwindow* window, double x, double y) {
	if (!TwMouseMotion(x, y)) {
		g_App->OnMouseMove(x, y);
	}
}

void glfw_OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
	g_App->OnMouseScroll(xOffset, yOffset);
}

void glfw_OnMouseClick(GLFWwindow* window, int button, int action, int mods) {
	
	TwMouseButtonID btn = (button == GLFW_MOUSE_BUTTON_LEFT) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	if (!TwMouseButton(ma, btn)) {
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
}

void glfw_OnKeyClick(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressed = (action == GLFW_PRESS);

	if (pressed) {
		int TwKey = glfwKey2TwKey(key);
		TwKeyPressed(TwKey, TW_KMOD_NONE);
	}

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

	if (!TwInit(TW_OPENGL_CORE, 0)) {
		std::cout << "Failed to initialize AntTweakBar!" << std::endl;
		result = false;
	}

	glViewport(0, 0, 800, 600);

	//初始化World并执行回调
	if (!GetWorld()->Initialize()) {
		std::cout << "Failed to initialize World in GLFWApplication!" << std::endl;
		result = false;
	}

	TwBar* bar = TwNewBar("JCore");
	TwWindowSize(800, 600);
	TwAddVarRW(bar, "normal", TW_TYPE_BOOLCPP, &GetWorld()->GetEnableRenderNormal(), "help='Turn On/Off normal'");
	TwAddVarRW(bar, "fly", TW_TYPE_BOOLCPP, &GetWorld()->GetFlyMode(), "help='Turn On/Off fly mode'");
	TwAddVarRW(bar, "light", TW_TYPE_BOOLCPP, &GetWorld()->GetEnableLight(), "help='Turn On/Off light'");
	TwAddVarRW(bar, "skybox", TW_TYPE_BOOLCPP, &GetWorld()->GetEnableSkybox(), "help='Turn On/Off skybox'");

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

		TwDraw();

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
