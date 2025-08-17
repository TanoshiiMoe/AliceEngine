#include "SpineDemoApp.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <Scene/SpineScene.h>

void SpineDemoApp::Initialize()
{
	__super::Initialize();
	SceneManager::AddScene<SpineScene>(L"SpineScene");
	SceneManager::ChangeScene(L"SpineScene");
}

void SpineDemoApp::Run()
{
	__super::Run();
	MSG msg = {};
	while (!m_quit)
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

void SpineDemoApp::Render()
{
	__super::Render();
}

void SpineDemoApp::Update()
{
	__super::Update();
}

void SpineDemoApp::Uninitialize()
{
	__super::Uninitialize();
} 