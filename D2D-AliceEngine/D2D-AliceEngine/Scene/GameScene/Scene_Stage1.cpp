﻿#include "Scene_Stage1.h"
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
#include "../../Scripts/BackGroundImage.h"
#include "../../Scripts/Audio.h"
#include "../../Scripts/UI_Script.h"
#include "../../Scripts/BackGroundVideo.h"
#include "../../Prefab/Truck.h"
#include "../../Scripts/BackGroundRender.h"
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include <TileMap/TileMapComponent.h>
#include <UI/UIButton.h>
#include <Component/BackGroundComponent.h>
#include <Component/SkewTransform.h>
#include <Effect/Prism.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/ButtonComponent.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Camera/CameraMover.h>
#include <Prefab/Player/PlayerBike.h>
#include "Scripts/TileMap/TileMapManager.h"
#include <GameManager/BulletManager.h>
#include "Scripts/Enemy/Spawn/EnemySpawner.h"
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Widget/StageWidgetScript.h>
#include <Scripts/Enemy/Spawn/SpawnCollider.h>
#include <GameManager/GamePlayManager.h>

void Scene_Stage1::Initialize()
{
	__super::Initialize();
}

void Scene_Stage1::Release()
{
	__super::Release();
}

void Scene_Stage1::Update()
{
	__super::Update();
}

void Scene_Stage1::OnEnter()
{
	__super::OnEnter();

	m_UI = NewObject<gameObject>(L"UI");
	//m_UI->AddComponent<TitleUIScript>();
	// 진입 시 글로벌 타임스케일 0으로 두고 컷씬 먼저 재생
	TimerManager::GetInstance().SetGlobalTimeScale(0.0f);
	// 컷씬 위젯 추가 및 다음 씬 이름 주입
	if (CutSceneWidgetScript* cut = m_UI->AddComponent<CutSceneWidgetScript>())
		cut->m_nextSceneName = m_nextSceneName;

	m_cameraController = NewObject<gameObject>(L"Camera");
	m_cameraController->AddComponent<CameraMover>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<PlayerBike>();
	BulletManager::GetInstance().SetPlayer(m_player);
	m_player->AddComponent<BackGroundRender>();

	//m_bg = NewObject<gameObject>(L"BackGround");
	//m_bg->AddComponent<BackGroundVideo>()->SetPlayer(m_player);

	// 오디오 추가, 오디오 관련 스크립트 넣기
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<Audio>();

	// 타일맵 추가
	m_tile = NewObject<gameObject>(L"TileMap");
	m_tile->AddComponent<TileMapComponent>();
	if (auto* tileMgr = m_tile->AddComponent<TileMapManager>())
	{
		tileMgr->SetTilePaths(
			L"TileMap/stage01_real/stage01_real.tmj",
			L"TileMap/stage01_real/Tile_Road.tsj",
			L"TileMap/TileMapColiderInfo.json");
	}

	// 적 스포너 매니저 생성
	gameObject* coll = NewObject<gameObject>(L"SpawnCollider");
	coll->AddComponent<SpawnCollider>();
	gameObject* eSpwaner = NewObject<gameObject>(L"EnemySpawner");
	eSpwaner->AddComponent<EnemySpawner>();

	// 이거 띄우면 적이 생성이 안되는데 확인 부탁드립니다
	m_button = NewObject<gameObject>(L"PauseButton");
	m_button->AddComponent<StageWidgetScript>();

	// Truck(점프대)
	m_truck = NewObject<gameObject>(L"Truck");
	m_truck->AddComponent<Truck>();

	// 디버그용 씬 전환
	gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
	sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_3)) {
			SceneManager::ChangeScene(L"TitleScene");
		}
	});

	gameObject* clearGame = NewObject<gameObject>(L"SceneChanger2");
	clearGame->AddComponent<InputComponent>()->SetAction(clearGame->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_4)) {
			GamePlayManager::GetInstance().GameClear();
		}
	});

	gameObject* overGame = NewObject<gameObject>(L"SceneChanger2");
	overGame->AddComponent<InputComponent>()->SetAction(overGame->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_5)) {
			GamePlayManager::GetInstance().GameOver();
		}
	});
}

void Scene_Stage1::OnExit()
{
	__super::OnExit();
}
