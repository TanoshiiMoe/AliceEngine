#pragma once
#pragma once
#include "../stdafx.h"
#include <Component/InputComponent.h>
#include <Object/gameObject.h>
#include <Manager/D2DRenderManager.h>
class CameraInputContext : public InputComponent
{
public:
	void Initialize() override
	{
		__super::Initialize();
	}
	void Update() override
	{
		__super::Update();

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
	void Release() override
	{
		__super::Release();
	}
};

