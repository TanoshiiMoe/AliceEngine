#include "DemoScene4.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Component/AudioComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Aru.h"
#include "../Scripts/Aru2.h"
#include "../Scripts/Player.h"
#include "../Scripts/Enemy.h"
#include "../Scripts/Audio.h"
#include "../Scripts/UI_Script.h"
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include <TileMap/TileMapComponent.h>
#include <Object/Canvas.h>

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
	SceneManager::GetCamera()->SetOwner(m_player);
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });
	m_player->AddComponent<Rigidbody2D>();
	m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;

	// 오디오 추가, 오디오 관련 스크립트 넣기
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<Audio>();

	m_tile = NewObject<gameObject>(L"TileMap");
	m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/test.tmj");
	
	// UI 추가
	//m_canvas = NewObject<Canvas>(L"Canvas");
	//m_canvas->AddUI<UI_Script>();

	m_UI = NewObject<gameObject>(L"ad");
	m_UI->AddComponent<UI_Script>();

	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));
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
