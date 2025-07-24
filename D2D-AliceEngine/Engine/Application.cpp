#pragma once
#include "pch.h"
#include "Application.h"
#include <Core/Input.h>
#include <Core/ObjectHandler.h>
#include <Helpers/FileHelper.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/TimerManager.h>
#include <Manager/ClassManager.h>
#include <Manager/AudioManager.h>
#include <Manager/UIManager.h>
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>
#include <System/RenderSystem.h>
#include <System/CollisionSystem.h>
#include <System/PhysicsSystem.h>
#include <Scene/Scene.h>
#include <Object/Camera.h>


Application::Application()
{
	m_hwnd = nullptr;
	m_hInstance = nullptr;
	FileHelper::CopyFilesToBuildPath(L"Resource");
	FileHelper::CopyFilesToBuildPath(L"Extension");
}

Application::~Application()
{
	ObjectHandler::Destroy();
	D2DRenderManager::Destroy();
	RenderSystem::Destroy();
	ScriptSystem::Destroy();
	InputSystem::Destroy();
	SceneManager::Destroy();
	CollisionSystem::Destroy();
	ClassManager::Destroy();
	PhysicsSystem::Destroy();
	AudioManager::Destroy();
	UIManager::Destroy();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* pThis = nullptr;

	// WM_NCCREATE: ������ ���� ���� �ʱ⿡, ������ ���� ����. WM_CREATE���� �������߻�
	if (msg == WM_NCCREATE)
	{
		// lParam�� CREATESTRUCT* �̴�
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<Application*>(cs->lpCreateParams);

		// HWND�� this ������ ����
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		// WM_NCCREATE�� �ƴ� ���� HWND���� this �����͸� �����´�
		pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis)
		pThis->MessageProc(hwnd, msg, wParam, lParam); // ��� �Լ� ȣ��

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::Initialize()
{
	ObjectHandler			::Create();
	ScriptSystem			::Create();
	InputSystem				::Create();
	D2DRenderManager		::Create();
	SceneManager			::Create();
	UpdateTaskManager		::Create();
	TimerManager			::Create();
	RenderSystem			::Create();
	CollisionSystem			::Create();
	PhysicsSystem			::Create();
	AudioManager			::Create();
	UIManager				::Create();

	char szPath[MAX_PATH] = { 0, };
	GetModuleFileNameA(NULL, szPath, MAX_PATH); // ���� ����� ���
	m_ModulePath = szPath; // ��� ���
	GetCurrentDirectoryA(MAX_PATH, szPath); // ���� �۾� ���丮
	m_WorkingPath = szPath; // �۾� ���丮
	OutputDebugStringA(std::string(std::string(m_ModulePath) + std::string("\n")).c_str());
	OutputDebugStringA(std::string(std::string(m_WorkingPath) + std::string("\n")).c_str());

	m_hInstance = GetModuleHandle(NULL); // ���� ���μ����� �ڵ�

	// WndProc�� ����ؾ��ؼ� ���⼭ �ʱ�ȭ.
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_WindowName.c_str();

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ���� ����
	RegisterClassEx(&wc);

	// ũ�⸦ ��������.
	SIZE clientSize = { (LONG)m_width,(LONG)m_height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(
		0,
		m_WindowName.c_str(),
		m_TitleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		nullptr,
		nullptr,
		m_hInstance,
		this // this �����͸� lpCreateParams�� ����
	);
	ShowWindow(m_hwnd, SW_SHOW);

	CoInitialize(nullptr);

	D2DRenderManager::GetInstance().Initialize(m_hwnd);

	Input::Initialize(m_hwnd);
	TimerManager::GetInstance().Initialize();
	TimerManager::GetInstance().UpdateTime();
	PackageResourceManager::Create();
	PackageResourceManager::GetInstance().Initialize();
	ClassManager::Create();
	ClassManager::GetInstance().RegisterClassInfo();
}

void Application::Run()
{

}

void Application::Update()
{
	TimerManager::GetInstance().UpdateTime();
	SceneManager::GetInstance().Update();
	Input::Update();
}

void Application::Input()
{

}

void Application::Render()
{
	RenderSystem::GetInstance().Render();
}

void Application::Uninitialize()
{
	D2DRenderManager::GetInstance().UnInitialize();
	SceneManager::GetInstance().UnInitialize();

	RenderSystem::GetInstance().UnRegistAll();
	ScriptSystem::GetInstance().UnRegistAll();
	InputSystem::GetInstance().UnRegistAll();
	CollisionSystem::GetInstance().UnRegistAll();
	PhysicsSystem::GetInstance().UnRegistAll();
	CoUninitialize();
}

void Application::MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			break; // �ּ�ȭ�� ����

		UINT width = LOWORD(lParam); // �� �ʺ�
		UINT height = HIWORD(lParam); // �� ����			
		if (m_width != width || m_height != height)
		{
			m_width = width;
			m_height = height;
			m_resized = true;
		}
	}
	break;
	default:
		break;
	}
}

void Application::GetApplicationSize(int& width, int& height)
{
	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
}