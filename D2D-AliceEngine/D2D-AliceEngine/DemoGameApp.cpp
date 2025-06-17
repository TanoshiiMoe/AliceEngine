#pragma once
#include "stdafx.h"
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

	Scene* SolarSystemScene = SceneManager::AddScene(L"SolarSystemScene");
	SceneManager::ChangeScene(L"SolarSystemScene");
    
	if (SolarSystemScene)
	{
		m_sun = SolarSystemScene->NewObject<gameObject>(L"Sun");
		m_sun->Initialize(L"Sun.png", FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_pD2DRenderManager->AddRenderer(m_sun->GetRenderer());

		m_earth = SolarSystemScene->NewObject<gameObject>(L"Earth");
		m_earth->Initialize(L"Earth.png", FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_sun->AddChildObject(m_earth);
		m_pD2DRenderManager->AddRenderer(m_earth->GetRenderer());

		m_moon = SolarSystemScene->NewObject<gameObject>(L"Moon");
		m_moon->Initialize(L"Moon.png", FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
		m_earth->AddChildObject(m_moon);
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
			Input();
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

	m_earth->m_localTransform.lock()->Rotation += 0.5f; // 지구 자전
	m_moon->m_localTransform.lock()->Rotation -= 2.0f; // 달 자전
	m_sun->m_localTransform.lock()->Rotation += 0.2f;
}

void DemoGameApp::Input()
{
	__super::Input();
    const std::vector<int> keys = {
        VK_Z, VK_C, VK_B, VK_M,
        VK_J, VK_G, VK_Y, VK_H,
        VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN,
        VK_1, VK_2,
        VK_W, VK_A, VK_S, VK_D,
        '9', '0', 'O', 'L', 'K', VK_OEM_1  // '9','0','O','L','K',';'
    };


	Camera* m_mainCamera = SceneManager::GetCamera();
    for (int key : keys)
    {
        if (GetAsyncKeyState(key) & 0x8000)
        {
            switch (key)
            {
            case VK_Z:
            {
                float rot = m_sun->m_localTransform.lock()->GetRotation();
                m_sun->m_localTransform.lock()->SetRotation(rot + 5.0f);
                break;
            }
            case VK_C:
            {
                float rot = m_sun->m_localTransform.lock()->GetRotation();
                m_sun->m_localTransform.lock()->SetRotation(rot - 5.0f);
                break;
            }
            case VK_B:
            {
                float rot = m_earth->m_localTransform.lock()->GetRotation();
                m_earth->m_localTransform.lock()->SetRotation(rot + 5.0f);
                break;
            }
            case VK_M:
            {
                float rot = m_earth->m_localTransform.lock()->GetRotation();
                m_earth->m_localTransform.lock()->SetRotation(rot - 5.0f);
                break;
            }
            case VK_J:
            {
                auto pos = m_earth->m_localTransform.lock()->GetPosition();
                m_earth->m_localTransform.lock()->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_G:
            {
                auto pos = m_earth->m_localTransform.lock()->GetPosition();
                m_earth->m_localTransform.lock()->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_Y:
            {
                auto pos = m_earth->m_localTransform.lock()->GetPosition();
                m_earth->m_localTransform.lock()->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case VK_H:
            {
                auto pos = m_earth->m_localTransform.lock()->GetPosition();
                m_earth->m_localTransform.lock()->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case VK_RIGHT:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_LEFT:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_UP:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case VK_DOWN:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case VK_1:
            {
                m_pD2DRenderManager->m_eTransformType = ETransformType::D2D;
                break;
            }
            case VK_2:
            {
                m_pD2DRenderManager->m_eTransformType = ETransformType::Unity;
                break;
            }
            case VK_D:
            {
                auto pos = m_sun->m_localTransform.lock()->GetPosition();
                m_sun->m_localTransform.lock()->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_A:
            {
                auto pos = m_sun->m_localTransform.lock()->GetPosition();
                m_sun->m_localTransform.lock()->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_W:
            {
                auto pos = m_sun->m_localTransform.lock()->GetPosition();
                m_sun->m_localTransform.lock()->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case VK_S:
            {
                auto pos = m_sun->m_localTransform.lock()->GetPosition();
                m_sun->m_localTransform.lock()->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }

            case '9':
            {
                float rot = m_moon->m_localTransform.lock()->GetRotation();
                m_moon->m_localTransform.lock()->SetRotation(rot + 5.0f);
                break;
            }
            case '0':
            {
                float rot = m_moon->m_localTransform.lock()->GetRotation();
                m_moon->m_localTransform.lock()->SetRotation(rot - 5.0f);
                break;
            }
            case 'O':
            {
                auto pos = m_moon->m_localTransform.lock()->GetPosition();
                m_moon->m_localTransform.lock()->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case 'L':
            {
                auto pos = m_moon->m_localTransform.lock()->GetPosition();
                m_moon->m_localTransform.lock()->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case 'K':
            {
                auto pos = m_moon->m_localTransform.lock()->GetPosition();
                m_moon->m_localTransform.lock()->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_OEM_1:  // ';' 키
            {
                auto pos = m_moon->m_localTransform.lock()->GetPosition();
                m_moon->m_localTransform.lock()->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            }
        }
    }

}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();
}