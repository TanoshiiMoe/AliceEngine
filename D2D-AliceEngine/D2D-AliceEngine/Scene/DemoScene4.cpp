#include "DemoScene4.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Aru.h"
#include "../Scripts/Aru2.h"
#include "../Scripts/Player.h"
#include "../Scripts/Enemy.h"
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

void DemoScene4::Initialize()
{
	__super::Initialize();
}

void DemoScene4::Release()
{
	__super::Release();
}

void DemoScene4::Update()
{
	__super::Update();
}

void DemoScene4::OnEnter()
{
	__super::OnEnter();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });
}

void DemoScene4::OnExit()
{
	__super::OnExit();
}

void DemoScene4::PlayerInput()
{
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
}

void DemoScene4::EnemyInput()
{
}
