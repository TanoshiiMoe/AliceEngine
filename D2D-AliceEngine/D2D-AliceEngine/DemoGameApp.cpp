#pragma once
#include "DemoGameApp.h"

DemoGameApp::DemoGameApp()
{

}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();
	m_pD2DRenderManager->Initialize(m_hwnd);

	m_solarSystemScene = std::make_shared<SolarSystemScene>();
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

void DemoGameApp::Input()
{
	__super::Input();
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();

	m_Sun->Release();
	m_Earth->Release();
	m_Moon->Release();

	m_pD2DRenderManager->Uninitialize();
	m_pD2DRenderManager = nullptr;
	CoUninitialize();
}