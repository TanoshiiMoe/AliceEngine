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
	//SceneManager::GetCamera()->SetOwner(m_player);
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });
	//m_player->AddComponent<Rigidbody2D>();

	// 오디오 추가, 오디오 관련 스크립트 넣기
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<Audio>();

	m_tile = NewObject<gameObject>(L"TileMap");
	//m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/test5.tmj");
	m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/BigMap.tmj");
	//m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/test4.tmj");
	m_tile->GetComponent<TileMapComponent>()->LoadTileSetData(L"TileMap/blocks.tsj");
	//m_tile->GetComponent<TileMapComponent>()->LoadTileSetData(L"TileMap/Tile_Road.tsj");
	m_tile->GetComponent<TileMapComponent>()->CreatetileRenderers();
	// UI 추가
	//m_canvas = NewObject<Canvas>(L"Canvas");
	//m_canvas->AddUI<UI_Script>();

	m_UI = NewObject<gameObject>(L"ad");
	m_UI->AddComponent<UI_Script>();

	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));

	//RemoveObject(m_wall);

	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput();  });
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
	if (Input::IsKeyDown(VK_K))
	{
		m_player->GetComponent<Player>()->walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_player->GetComponent<Player>()->walkSpeed -= 5.0f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_player->GetComponent<Player>()->bMoveRigidBody = !m_player->GetComponent<Player>()->bMoveRigidBody;
	}

	if (Input::IsKeyDown(VK_D))
	{
		SceneManager::GetCamera()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_A))
	{
		SceneManager::GetCamera()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_W))
	{
		SceneManager::GetCamera()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_S))
	{
		SceneManager::GetCamera()->AddPosition(0, -5.0f);
	}
}

void DemoScene4::EnemyInput()
{
}
