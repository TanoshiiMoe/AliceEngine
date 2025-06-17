#pragma once
#include "pch.h"
#include "Manager/D2DRenderManager.h"
#include "System/RenderSystem.h"
#include "Manager/SceneManager.h"
#include <Core/Singleton.h>
#include <Object/Camera.h>
#include "Scene/Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>

using namespace Microsoft::WRL;

class Application : public Singleton<Application>
{
public:
	Application();
	~Application();

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render();
	virtual void Update();
	virtual void Input();
	virtual void Uninitialize();

	D2DRenderManager* m_pD2DRenderManager;
	RenderSystem* m_pRenderSystem;
	ScriptSystem* m_pScriptSystem;
	InputSystem* m_pInputSystem;
	TransformSystem* m_pTransformSystem;
	SceneManager* m_pSceneManager;

protected:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hInstance;
	std::wstring	m_WindowName = Define::WINDOW_CLASS_NAME;
	std::wstring	m_TitleName = Define::WINDOW_TITLE;
	std::string		m_ModulePath;
	std::string		m_WorkingPath;

	UINT m_width = Define::SCREEN_WIDTH;
	UINT m_height = Define::SCREEN_HEIGHT;
	bool m_resized = false;

public:
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#define GetApplication() Application::Get()