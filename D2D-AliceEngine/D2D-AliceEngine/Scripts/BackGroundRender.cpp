#include "BackGroundRender.h"
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
#include "Bike/BikeMovementScript.h"

void BackGroundRender::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void BackGroundRender::FixedUpdate(const float& deltaSeconds)
{
}

void BackGroundRender::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 플레이어 이동 스크립트 캐싱
	if (!m_playerMove)
	{
		WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player");
		if (!player.expired())
			m_playerMove = player.Get()->GetComponent<BikeMovementScript>();
	}

	float playerSpeed = 0.0f;
	if (m_playerMove)
		playerSpeed = m_playerMove->GetCurrentSpeed() * m_playerMove->GetSpeedModifierValue();

	// ScreenSpace로 그리므로 카메라 이동과 무관, X만 패럴랙스 스크롤
	for (auto& layer : m_loopingLayers)
	{
		if (!layer.obj1 || !layer.obj2) continue;

		FVector2 pos1 = layer.obj1->transform()->GetPosition();
		FVector2 pos2 = layer.obj2->transform()->GetPosition();

		float move = playerSpeed * layer.speed * deltaSeconds;
		pos1.x -= move;
		pos2.x -= move;

		const float restX = layer.width;
		if (pos1.x <= -restX)
			pos1.x = pos2.x + restX;
		else if (pos2.x <= -restX)
			pos2.x = pos1.x + restX;

		layer.obj1->transform()->SetPosition(FVector2(pos1.x, layer.y));
		layer.obj2->transform()->SetPosition(FVector2(pos2.x, layer.y));
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
	//GetCamera()->AddChildObject(m_owner);

	// 패럴랙스 레이어 구성 (위 3, 아래 3)
	AddLooping(L"Sky",       L"BackGround\\BG_Sky.png",       -10, 550.0f, 0.2f); // 가장 먼 상단
	AddLooping(L"Clouds",    L"BackGround\\BG_Building.png",  -7,  470.0f, 0.4f); // 중간 상단(임시 리소스)
	AddLooping(L"Building",  L"BackGround\\BG_Building.png",  -5,  390.0f, 0.6f); // 가까운 상단

	//m_bridge = GetWorld()->NewObject<gameObject>(L"Bridge");
	//auto bridge = m_bridge->AddComponent<SpriteRenderer>();
	//bridge->LoadData(L"BackGround\\BG_Bridge.png");
	//bridge->SetDrawType(EDrawType::ScreenSpace);
	//bridge->SetRelativePosition(FVector2(bridge->GetBitmapSizeX() / 2.0f, 1190));
	//bridge->m_layer = 4;

	//m_market = GetWorld()->NewObject<gameObject>(L"Market");
	//auto market = m_market->AddComponent<SpriteRenderer>();
	//market->LoadData(L"BackGround\\BG_Market.png");
	//market->SetDrawType(EDrawType::ScreenSpace);
	//market->SetRelativePosition(FVector2(market->GetBitmapSizeX() / 2.0f, 1140));
	//market->m_layer = 5;

	//m_backBarrier = GetWorld()->NewObject<gameObject>(L"BackBarrier");
	//auto backBarrier = m_backBarrier->AddComponent<SpriteRenderer>();
	//backBarrier->LoadData(L"BackGround\\BG_BackBarrier.png");
	//backBarrier->SetDrawType(EDrawType::ScreenSpace);
	//backBarrier->SetRelativePosition(FVector2(backBarrier->GetBitmapSizeX() / 2.0f, 250));
	//backBarrier->m_layer = 10;

	//m_frontBarrier = GetWorld()->NewObject<gameObject>(L"FrontBarrier");
	//auto frontBarrier = m_frontBarrier->AddComponent<SpriteRenderer>();
	//frontBarrier->LoadData(L"BackGround\\BG_Barrier.png");
	//frontBarrier->SetDrawType(EDrawType::ScreenSpace);
	//frontBarrier->SetRelativePosition(FVector2(frontBarrier->GetBitmapSizeX() / 2.0f, -200));
	//frontBarrier->m_layer = 11;

	//m_guardrail = GetWorld()->NewObject<gameObject>(L"GuardRail");
	//auto guardrail = m_guardrail->AddComponent<SpriteRenderer>();
	//guardrail->LoadData(L"BackGround\\BG_GuardRail.png");
	//guardrail->SetDrawType(EDrawType::ScreenSpace);
	//guardrail->SetRelativePosition(FVector2(guardrail->GetBitmapSizeX() / 2.0f, 240));
	//guardrail->m_layer = 12;
	WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player");
	if (!player.expired())
	{
		BikeMovementScript* bs = player.Get()->GetComponent<BikeMovementScript>();
		// 여기에 Get함수로 속도 받아오기
	}
	AddLooping(L"BackBarrier", L"BackGround\\BG_BackBarrier.png", 10, 250.0f, 0.9f); // 중간 하단
	AddLooping(L"GuardRail",  L"BackGround\\BG_GuardRail.png",  12, 310.0f, 1.0f); // 가장 가까운 하단
	//AddLooping(L"FrontBarrier", L"BackGround\\BG_Barrier.png", 13, 740.0f, 1.2f);
}

void BackGroundRender::OnEnd()
{
}

void BackGroundRender::OnDestroy()
{
}

void BackGroundRender::AddLooping(const std::wstring& name, const std::wstring& texturePath, int layer, float y, float speed)
{
	auto obj1 = GetWorld()->NewObject<gameObject>(name + L"_1");
	auto* sr1 = obj1->AddComponent<SpriteRenderer>();
	//GetCamera()->AddChildObject(obj1);
	sr1->LoadData(texturePath);
	sr1->SetDrawType(EDrawType::ScreenSpace);
	float width = sr1->GetBitmapSizeX();
	sr1->m_layer = layer;

	auto obj2 = GetWorld()->NewObject<gameObject>(name + L"_2");
	auto* sr2 = obj2->AddComponent<SpriteRenderer>();
	//GetCamera()->AddChildObject(obj2);
	sr2->LoadData(texturePath);
	sr2->SetDrawType(EDrawType::ScreenSpace);
	sr2->m_layer = layer;

	sr1->SetRelativePosition(FVector2(0, y));
	sr2->SetRelativePosition(FVector2(width, y));

	m_loopingLayers.push_back({ obj1, obj2, speed, width, y });
}
