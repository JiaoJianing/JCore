#pragma once
#include "Application.h"

class GlfwApplication : public Application
{
public:
	GlfwApplication();
	~GlfwApplication();

	virtual bool Initialize(int windowWidth, int windowHeight, const std::string& appName);

	virtual void Run();

	virtual void OnResize(int width, int height);

	virtual void OnMouseMove(double x, double y);

	virtual void OnMouseScroll(double xOffset, double yOffset);

	virtual void OnLButtonDown(double x, double y);

	virtual void OnLButtonUp(double x, double y);

	virtual void OnKeyboard(int key, bool pressed);

private:
	GLFWwindow * m_GlfwWindow;
};

