#pragma once
#include "DemoGameApp.h"
#include "Scene/DemoScene.h"
#include "Scene/DemoScene2.h"
#include "Scene/DemoScene3.h"
#include "Scene/DemoScene4.h"
#include "Scene/SpineScene.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>

/*-----���� �ΰ��Ӿ��� �߰��մϴ�-----*/
#include <Scene/TitleScene.h>
#include <Scene/SelectScene.h>
#include <Scene/KangScene/KangScene.h>
#include <Scripts/Weapon/BulletManager.h>
#include <GameManager/GamePlayManager.h>

DemoGameApp::DemoGameApp()
{
	BulletManager::Create();
}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();

	GamePlayManager::Create();
	SceneManager::AddScene<DemoScene2>(L"aruScene");
	SceneManager::AddScene<DemoScene3>(L"FSMScene");
	SceneManager::AddScene<DemoScene>(L"SolarSystemScene");
	SceneManager::AddScene<DemoScene4>(L"HiroScene");
	SceneManager::AddScene<TitleScene>(L"TitleScene");		// title
	SceneManager::AddScene<SpineScene>(L"SpineScene");
	SceneManager::AddScene<KangScene>(L"KangTest");
	SceneManager::AddScene<SelectScene>(L"SelectScene");	// stage select
	SceneManager::ChangeScene(L"TitleScene");
}

void DemoGameApp::Run()
{
	__super::Run();
	// �޽��� ����
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
