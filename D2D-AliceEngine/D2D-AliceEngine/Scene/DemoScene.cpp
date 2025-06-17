#include "DemoScene.h"
#include <Component/InputComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 weak_ptr를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene::Initialize()
{
}

void DemoScene::Release()
{
}

void DemoScene::Update()
{
	__super::Update();
	m_earth->m_transformComponent.lock()->m_localTransform->Rotation += 0.5f; // 지구 자전
	m_moon->m_transformComponent.lock()->m_localTransform->Rotation -= 2.0f; // 달 자전
	m_sun->m_transformComponent.lock()->m_localTransform->Rotation += 0.2f;
}

void DemoScene::OnEnter()
{
	__super::OnEnter();

	m_sun = AddObject<gameObject>(L"Sun");
	m_sun->Initialize(L"Sun.png", FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));

	m_earth = AddObject<gameObject>(L"Earth");
	m_earth->Initialize(L"Earth.png", FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));

	m_moon = AddObject<gameObject>(L"Moon");
	m_moon->Initialize(L"Moon.png", FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));

	m_sun->m_transformComponent.lock()->AddChildObject(m_earth->m_transformComponent);
	m_earth->m_transformComponent.lock()->AddChildObject(m_moon->m_transformComponent);

	m_sun->AddComponent<InputComponent>().lock()->SetAction([this]() { SunInput(); });
	m_earth->AddComponent<InputComponent>().lock()->SetAction([this]() { EarthInput(); });
	m_moon->AddComponent<InputComponent>().lock()->SetAction([this]() { MoonInput(); });
	m_sun->AddComponent<InputComponent>().lock()->SetAction([this]() { CameraInput(); });
}

void DemoScene::OnExit()
{
	__super::OnExit();
}

void DemoScene::SunInput()
{
	if (GetAsyncKeyState(VK_Z) & 0x8000)
	{
		float rot = m_sun->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_sun->m_transformComponent.lock()->m_localTransform->SetRotation(rot + 5.0f);
	}
	if (GetAsyncKeyState(VK_C) & 0x8000)
	{
		float rot = m_sun->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_sun->m_transformComponent.lock()->m_localTransform->SetRotation(rot - 5.0f);
	}
	if (GetAsyncKeyState(VK_D) & 0x8000)
	{
		auto pos = m_sun->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_sun->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_A) & 0x8000)
	{
		auto pos = m_sun->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_sun->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_W) & 0x8000)
	{
		auto pos = m_sun->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_sun->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
	}
	if (GetAsyncKeyState(VK_S) & 0x8000)
	{
		auto pos = m_sun->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_sun->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
	}
}

void DemoScene::MoonInput()
{
	if (GetAsyncKeyState('9') & 0x8000)
	{
		float rot = m_moon->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_moon->m_transformComponent.lock()->m_localTransform->SetRotation(rot + 5.0f);
	}
	if (GetAsyncKeyState('0') & 0x8000)
	{
		float rot = m_moon->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_moon->m_transformComponent.lock()->m_localTransform->SetRotation(rot - 5.0f);
	}
	if (GetAsyncKeyState(VK_O) & 0x8000)
	{
		auto pos = m_moon->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_moon->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
	}
	if (GetAsyncKeyState(VK_L) & 0x8000)
	{
		auto pos = m_moon->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_moon->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
	}
	if (GetAsyncKeyState(VK_K) & 0x8000)
	{
		auto pos = m_moon->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_moon->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_OEM_1) & 0x8000)
	{
		auto pos = m_moon->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_moon->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
	}
}

void DemoScene::EarthInput()
{
	if (GetAsyncKeyState(VK_B) & 0x8000)
	{
		float rot = m_earth->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_earth->m_transformComponent.lock()->m_localTransform->SetRotation(rot + 5.0f);
	}
	if (GetAsyncKeyState(VK_M) & 0x8000)
	{
		float rot = m_earth->m_transformComponent.lock()->m_localTransform->GetRotation();
		m_earth->m_transformComponent.lock()->m_localTransform->SetRotation(rot - 5.0f);
	}
	if (GetAsyncKeyState(VK_J) & 0x8000)
	{
		auto pos = m_earth->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_earth->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_G) & 0x8000)
	{
		auto pos = m_earth->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_earth->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_Y) & 0x8000)
	{
		auto pos = m_earth->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_earth->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
	}
	if (GetAsyncKeyState(VK_H) & 0x8000)
	{
		auto pos = m_earth->m_transformComponent.lock()->m_localTransform->GetPosition();
		m_earth->m_transformComponent.lock()->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
	}
}

void DemoScene::CameraInput()
{
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		auto pos = SceneManager::GetCamera()->m_transform->GetPosition();
		SceneManager::GetCamera()->m_transform->SetPosition(pos.x + 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		auto pos = SceneManager::GetCamera()->m_transform->GetPosition();
		SceneManager::GetCamera()->m_transform->SetPosition(pos.x - 5.0f, pos.y);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		auto pos = SceneManager::GetCamera()->m_transform->GetPosition();
		SceneManager::GetCamera()->m_transform->SetPosition(pos.x, pos.y + 5.0f);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		auto pos = SceneManager::GetCamera()->m_transform->GetPosition();
		SceneManager::GetCamera()->m_transform->SetPosition(pos.x, pos.y - 5.0f);
	}
	if (GetAsyncKeyState(VK_1) & 0x8000)
	{
		D2DRenderManager::Get().m_eTransformType = ETransformType::D2D;
	}
	if (GetAsyncKeyState(VK_2) & 0x8000)
	{
		D2DRenderManager::Get().m_eTransformType = ETransformType::Unity;
	}
}
