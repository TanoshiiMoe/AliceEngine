#include "Scene_Stage3.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Object/gameObject.h>
#include <Component/TileMapRenderer.h>
#include <TileMap/TileMapComponent.h>
#include <Scripts/TileMap/TileMapManager.h>

#include <Manager/TimerManager.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Camera/CameraMover.h>
#include <Prefab/Player/PlayerBike.h>
#include <Scripts/BackGroundRender.h>
#include <Scripts/Audio.h>
#include <Scripts/Enemy/Spawn/EnemySpawner.h>
#include <Prefab/Truck.h>
#include <GameManager/BulletManager.h>

void Scene_Stage3::Initialize()
{
    __super::Initialize();
}

void Scene_Stage3::Release()
{
    __super::Release();
}

void Scene_Stage3::Update()
{
    __super::Update();
}

void Scene_Stage3::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"Stage3Label");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();
    text->SetText(L"<현재 씬> Scene_Stage3");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(24.0f);

    // 타일맵 추가
    m_tile = NewObject<gameObject>(L"TileMap");
    m_tile->AddComponent<TileMapComponent>();
    if (auto* tileMgr = m_tile->AddComponent<TileMapManager>())
    {
        tileMgr->SetTilePaths(
            L"TileMap/stage03_real/stage_shooting_003.tmj",
            L"TileMap/stage03_real/Tile_Road_00.tsj",
            L"TileMap/TileMapColiderInfo.json");
    }

    // VK_3 누르면 TitleScene으로 전환
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(L"TitleScene");
        }
    });


    //=================================== 게임 로직 
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

    // 적 스포너 매니저 생성
    gameObject* eSpwaner = NewObject<gameObject>(L"EnemySpawner");
    eSpwaner->AddComponent<EnemySpawner>();

    // 이거 띄우면 적이 생성이 안되는데 확인 부탁드립니다
    //m_button = NewObject<gameObject>(L"PauseButton");
    //m_button->AddComponent<StageWidgetScript>();

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
}

void Scene_Stage3::OnExit()
{
    __super::OnExit();
}


