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
#include <Manager/PackageResourceManager.h>

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
	PackageResourceManager* m_pPackageResourceManager;
	SceneManager* m_pSceneManager;
	RenderSystem* m_pRenderSystem;
	ScriptSystem* m_pScriptSystem;
	InputSystem* m_pInputSystem;
	TransformSystem* m_pTransformSystem;

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
	void GetApplicationSize(int& width, int& height);
};
#define GetApplication() Application::Get()