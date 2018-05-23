#include "stdafx.h"
#include "Application.h"

Application* g_App = 0;

Application::Application()
	: m_WindowWidth(0)
	, m_WindowHeight(0)
	, m_World(0)
	, OnInitializeCallback(0)
	, OnUpdateCallback(0)
	, OnKeyboardCallback(0)
	, OnMouseDownCallback(0)
	, m_DeltaFrame(0)
	, m_LastFrame(0)
{
}


Application::~Application()
{
	if (m_World != 0)
	{
		delete m_World;
		m_World = 0;
	}
}

bool Application::Initialize(int windowWidth, int windowHeight, const std::string& appName)
{
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
	m_AppName = appName;
	m_World = new World(windowWidth, windowHeight);

	return true;
}

int Application::GetWindowWidth()
{
	return m_WindowWidth;
}

void Application::SetWindowWidth(int width)
{
	m_WindowWidth = width;
}

int Application::GetWindowHeight()
{
	return m_WindowHeight;
}

void Application::SetWindowHeight(int height)
{
	m_WindowHeight = height;
}

std::string& Application::GetAppName()
{
	return m_AppName;
}

World* Application::GetWorld()
{
	return m_World;
}
