#pragma once
#include "DemoGameApp.h"
#include "Scene/DemoScene.h"
#include "Scene/DemoScene2.h"
#include "Scene/DemoScene3.h"
#include "Scene/DemoScene4.h"
#include <Manager/SceneManager.h>

DemoGameApp::DemoGameApp()
{

}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();

	SceneManager::AddScene<DemoScene2>(L"aruScene");
	SceneManager::AddScene<DemoScene3>(L"FSMScene");
	SceneManager::AddScene<DemoScene>(L"SolarSystemScene");
	SceneManager::AddScene<DemoScene4>(L"HiroScene");
	SceneManager::ChangeScene(L"FSMScene");
}

void DemoGameApp::Run()
{
	__super::Run();
	// 메시지 루프
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}
}

void DemoGameApp::Render()
{
	__super::Render();
}

void DemoGameApp::Update()
{
	__super::Update();
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();
}