#pragma once
#include "DemoGame.h"

DemoGame::DemoGame()
{

}

DemoGame::~DemoGame()
{

}

void DemoGame::Initialize()
{
	__super::Initialize();
	m_pD2DRenderManager->Initialize(m_hwnd);

	//태양
	m_Sun = std::make_shared<Object>(L"Sun.png", FVector2(0,0), 0.0f, FVector2(0.5f,0.5f), FVector2(0.5f));
	m_pD2DRenderManager->AddRenderer(m_Sun->GetRenderer());

	// 지구
	m_Earth = std::make_shared<Object>(L"Earth.png", FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_Sun->m_bitmapRenderer->AddChild(m_Earth->m_bitmapRenderer->weak_from_this());
	m_pD2DRenderManager->AddRenderer(m_Earth->GetRenderer());

	// 달
	m_Moon = std::make_shared<Object>(L"Moon.png", FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_Earth->m_bitmapRenderer->AddChild(m_Moon->m_bitmapRenderer->weak_from_this());
	m_pD2DRenderManager->AddRenderer(m_Moon->GetRenderer());
}

void DemoGame::Run()
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

void DemoGame::Render()
{
	__super::Render();
}

void DemoGame::Update()
{
	__super::Update();

	m_Earth->m_bitmapRenderer->m_localTransform->Rotation += 0.5f; // 지구 자전
	m_Moon->m_bitmapRenderer->m_localTransform->Rotation -= 2.0f; // 달 자전
	m_Sun->m_bitmapRenderer->m_localTransform->Rotation += 0.2f;
	m_Sun->Update();
}

void DemoGame::Uninitialize()
{
	__super::Uninitialize();

	m_Sun->Release();
	m_Earth->Release();
	m_Moon->Release();

	m_pD2DRenderManager->Uninitialize();
	m_pD2DRenderManager = nullptr;
	CoUninitialize();
}