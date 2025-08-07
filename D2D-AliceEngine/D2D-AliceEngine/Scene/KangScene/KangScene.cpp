#include "KangScene.h"
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
#include "../../Scripts/Player.h"
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
#include <Scripts/Weapon/BulletManager.h>

void KangScene::Initialize()
{
	__super::Initialize();
}

void KangScene::Release()
{
	__super::Release();
}

void KangScene::Update()
{
	__super::Update();
}

void KangScene::OnEnter()
{
	__super::OnEnter();

	m_cameraController = NewObject<gameObject>(L"Camera");
	m_cameraController->AddComponent<CameraMover>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<PlayerBike>();
	BulletManager::GetInstance().SetPlayer(m_player);

	//m_bg = NewObject<gameObject>(L"BackGround");
	//m_bg->AddComponent<BackGroundVideo>()->SetPlayer(m_player);

	// 오디오 추가, 오디오 관련 스크립트 넣기
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<Audio>();


	// 타일맵 추가
	m_tile = NewObject<gameObject>(L"TileMap");
	m_tile->AddComponent<TileMapComponent>();
	m_tile->AddComponent<TileMapManager>();

	// 적 스포너 매니저 생
	
	// Truck(점프대)
	m_truck = NewObject<gameObject>(L"Truck");
	m_truck->AddComponent<Truck>();
}

void KangScene::OnExit()
{
	__super::OnExit();
}
