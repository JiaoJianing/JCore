#pragma once
#include "World.h"
#include <string>

typedef void (*OnInitializeCb)(World* world);
typedef void (*OnUpdateCb)(World* world, float currentFrame, float deltaFrame);
typedef void(*OnKeyboardCb)(World* world, int key, bool pressed);
typedef void(*OnMouseDownCb)(World* world, double x, double y);

class Application
{
public:
	Application();
	virtual ~Application();

	virtual bool Initialize(int windowWidth, int windowHeight, const std::string& appName);

	virtual void Run() = 0;

	virtual void OnResize(int width, int height) = 0;

	virtual void OnMouseMove(double x, double y) = 0;

	virtual void OnMouseScroll(double xOffset, double yOffset) = 0;

	virtual void OnLButtonDown(double x, double y) = 0;

	virtual void OnLButtonUp(double x, double y) = 0;

	virtual void OnKeyboard(int key, bool pressed) = 0;

public:
	int GetWindowWidth();
	void SetWindowWidth(int width);
	
	int GetWindowHeight();
	void SetWindowHeight(int height);

	std::string& GetAppName();
	
	World* GetWorld();

	OnInitializeCb OnInitializeCallback;

	OnUpdateCb OnUpdateCallback;

	OnKeyboardCb OnKeyboardCallback;

	OnMouseDownCb OnMouseDownCallback;

protected:
	int m_WindowWidth;
	int m_WindowHeight;
	std::string m_AppName;
	World* m_World;
	float m_DeltaFrame;
	float m_LastFrame;
};

extern Application* g_App;

