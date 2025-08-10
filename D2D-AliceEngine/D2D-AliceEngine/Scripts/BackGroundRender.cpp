﻿#include "BackGroundRender.h"
#include <Manager/SceneManager.h>
#include <Scene/Scene.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>
#include <Helpers/CoordHelper.h>
#include <Component/BackGroundComponent.h>
#include <cmath>
#include "Bike/BikeMovementScript.h"

void BackGroundRender::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

    // 컬링 스케일을 넉넉하게 키움
    if (auto cam = SceneManager::GetCamera())
    {
        cam->SetCullingScale(2.0f, 1.5f);
    }
}

void BackGroundRender::FixedUpdate(const float& deltaSeconds)
{
}

void BackGroundRender::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 플레이어 참조를 캐싱해두자
	static gameObject* playerObj = nullptr;
	static BikeMovementScript* playerMove = nullptr;
	if (!playerObj)
	{
		WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player");
		if (!player.expired())
		{
			playerObj = player.Get();
			playerMove = playerObj->GetComponent<BikeMovementScript>();
		}
	}

	float playerX = 0.0f;
	float playerSpeed = 0.0f;
	if (playerObj && playerObj->transform())
		playerX = playerObj->transform()->GetPosition().x;
	if (playerMove)
		playerSpeed = playerMove->GetCurrentSpeed() * playerMove->GetSpeedModifierValue();

    // tiles 기반으로 +x 방향 진행, 플레이어 기준 뒤로 흐르는 효과
    for (auto& layer : m_loopingLayers)
    {
        if (layer.tiles.size() < 3) continue;

        float width = layer.width;
        if (width <= 0.0f)
        {
            if (auto sr = layer.tiles[0]->GetComponent<SpriteRenderer>())
                width = sr->GetBitmapSizeX();
            layer.width = width;
        }

        // 플레이어 속도에 비례해 배경을 왼쪽으로 이동
        const float move = playerSpeed * layer.speed * deltaSeconds;
        for (auto* tile : layer.tiles)
        {
            if (!tile) continue;
            FVector2 p = tile->transform()->GetPosition();
            p.x -= move;
            tile->transform()->SetPosition(FVector2(std::round(p.x), layer.y));
        }

        // 2→3 경계에서 1을 3의 오른쪽 끝 + width/2로 이동
        gameObject* t1 = layer.tiles[0];
        gameObject* t2 = layer.tiles[1];
        gameObject* t3 = layer.tiles[2];
        FVector2 p2 = t2->transform()->GetPosition();
        FVector2 p3 = t3->transform()->GetPosition();
        const float seam = 0.5f * (p2.x + p3.x);
        const float prevSeam = seam + move;
        if (playerX >= seam && playerX >= prevSeam)
        {
            const float w3 = t3->GetComponent<SpriteRenderer>()->GetBitmapSizeX();
            const float w1 = t1->GetComponent<SpriteRenderer>()->GetBitmapSizeX();
            const float newX = p3.x + 0.5f * (w3 + w1);
            FVector2 p1 = t1->transform()->GetPosition();
            p1.x = newX;
            t1->transform()->SetPosition(FVector2(std::round(p1.x), layer.y));

            // (1,2,3) -> (2,3,1) 로테이트
            // 오케이 여기서 위치를 옮기자 
            std::rotate(layer.tiles.begin(), layer.tiles.begin() + 1, layer.tiles.begin() + 3);
        }
    }
}

void BackGroundRender::LateUpdate(const float& deltaSeconds)
{
}

void BackGroundRender::Awake()
{
}

void BackGroundRender::OnStart()
{
	m_owner = GetOwner();

    // y=0을 중심으로 위(+Y) 3장, 아래(-Y) 3장 구성해보자
    // speed 파라미터는 계수 플레이어 속도에 비례해서 이동하게
    AddLooping(L"TopFar",    L"BackGround\\BG_Sky.png",         -10 + m_backgroundRelativeLayer, 400.0f, 0.2f);
    AddLooping(L"TopMid",    L"BackGround\\BG_Building.png",    -7  + m_topRelativeLayer, 500.0f, 0.4f);
    AddLooping(L"TopNear",   L"BackGround\\BG_BackBarrier.png", 5  + m_topRelativeLayer, 330.0f, 1.5f);
    // =========================== Player가 여기에 있고, 위 아래 배경이라는 뜻 ===================================
    AddLooping(L"BotNear",   L"BackGround\\BG_GuardRail.png",  5  + m_bottomRelativeLayer, -180.0f, 1.5f);
    AddLooping(L"BotMid",    L"BackGround\\BG_Bridge.png",     3  + m_bottomRelativeLayer, -640.0f, 0.9f);
    AddLooping(L"BotFar",    L"BackGround\\BG_Market.png",     -2   + m_backgroundRelativeLayer, -440.0f, 0.8f);
}

void BackGroundRender::OnEnd()
{
}

void BackGroundRender::OnDestroy()
{
}

void BackGroundRender::AddLooping(const std::wstring& name, const std::wstring& texturePath, int layer, float y, float speed)
{
    // 3장만 생성하여 순환(1,2,3)
    LoopingBackGround bg;
    bg.speed = speed;
    bg.y = y;
    bg.baseLayer = layer - m_backgroundRelativeLayer;
    bg.x = 0.0f;

    auto makeTile = [&](int idx, float xpos)
    {
        auto obj = GetWorld()->NewObject<gameObject>(name + L"_" + std::to_wstring(idx));
        auto* sr = obj->AddComponent<SpriteRenderer>();
        sr->LoadData(texturePath);
        sr->SetDrawType(EDrawType::WorldSpace);
        sr->m_layer = layer + m_backgroundRelativeLayer;
        obj->transform()->SetPosition(FVector2(xpos, y));
        bg.tiles.push_back(obj);
        if (bg.width <= 0.0f) bg.width = sr->GetBitmapSizeX();
    };

    // 플레이어 X 기준 정렬: [1,2,3]을 오른쪽으로 이어서 배치
    float playerX0 = 0.0f;
    {
        WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player");
        if (!player.expired() && player.Get()->transform())
            playerX0 = player.Get()->transform()->GetPosition().x;
    }

    makeTile(1, playerX0);
    makeTile(2, playerX0 + bg.width);
    makeTile(3, playerX0 + 2.0f * bg.width);

    m_loopingLayers.push_back(std::move(bg));
}
