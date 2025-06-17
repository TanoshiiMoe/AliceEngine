#pragma once
#include "stdafx.h"
#include "DemoGameApp.h"
#include "Input/SunInputContext.h"
#include "Input/MoonInputContext.h"
#include "Input/EarthInputContext.h"
#include "Input/CameraInputContext.h"

DemoGameApp::DemoGameApp()
{

}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();

	Scene* SolarSystemScene = SceneManager::AddScene(L"SolarSystemScene");
	SceneManager::ChangeScene(L"SolarSystemScene");

	if (SolarSystemScene)
	{
		m_sun = SolarSystemScene->NewObject<gameObject>(L"Sun");
		m_sun->Initialize(L"Sun.png", FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_sun->AddComponent<SunInputContext>();
		m_sun->AddComponent<CameraInputContext>();
		m_pD2DRenderManager->AddRenderer(m_sun->GetRenderer());

		m_earth = SolarSystemScene->NewObject<gameObject>(L"Earth");
		m_earth->Initialize(L"Earth.png", FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_earth->AddComponent<EarthInputContext>();
		m_sun->m_transformComponent.lock()->AddChildObject(m_earth->m_transformComponent);
		m_pD2DRenderManager->AddRenderer(m_earth->GetRenderer());

		m_moon = SolarSystemScene->NewObject<gameObject>(L"Moon");
		m_moon->Initialize(L"Moon.png", FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_moon->AddComponent<MoonInputContext>();
		m_earth->m_transformComponent.lock()->AddChildObject(m_moon->m_transformComponent);
		m_pD2DRenderManager->AddRenderer(m_moon->GetRenderer());
	}
	
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

	m_earth->m_transformComponent.lock()->m_localTransform->Rotation += 0.5f; // 지구 자전
	m_moon->m_transformComponent.lock()->m_localTransform->Rotation -= 2.0f; // 달 자전
	m_sun->m_transformComponent.lock()->m_localTransform->Rotation += 0.2f;
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();
}