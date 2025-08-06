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

	float playerPosY = m_owner->transform()->GetPosition().y;

	//for (auto& layer : m_loopingLayers)
	//{
	//	if (!layer.obj1 || !layer.obj2) return;

	//	FVector2 pos1 = layer.obj1->transform()->GetPosition();
	//	FVector2 pos2 = layer.obj2->transform()->GetPosition();

	//	pos1.x -= layer.speed * deltaSeconds;
	//	pos2.x -= layer.speed * deltaSeconds;

	//	float restX = layer.width;

	//	if (pos1.x <= -restX)
	//		pos1.x = pos2.x + restX;
	//	else if (pos2.x <= -restX)
	//		pos2.x = pos1.x + restX;

	//	layer.obj1->transform()->SetPosition(FVector2(pos1.x, pos1.y));
	//	layer.obj2->transform()->SetPosition(FVector2(pos2.x, pos2.y));
	//}
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

	m_sky = GetWorld()->NewObject<gameObject>(L"Sky");
	GetCamera()->AddChildObject(m_sky);
	auto sky = m_sky->AddComponent<SpriteRenderer>();
	sky->LoadData(L"BackGround\\BG_Sky.png");
	sky->SetDrawType(EDrawType::ScreenSpace);
	sky->SetRelativePosition(FVector2(960,550));
	sky->m_layer = -10;

	m_building = GetWorld()->NewObject<gameObject>(L"Building");
	//GetCamera()->AddChildObject(m_building);
	auto building = m_building->AddComponent<SpriteRenderer>();
	building->LoadData(L"BackGround\\BG_Building.png");
	building->SetDrawType(EDrawType::ScreenSpace);
	float width = building->GetBitmapSizeX();
	building->SetRelativePosition(FVector2(0, 390));
	building->m_layer = -5;
	
	auto m_building2 = GetWorld()->NewObject<gameObject>(L"Building2");
	//GetCamera()->AddChildObject(m_building);
	auto building2 = m_building2->AddComponent<SpriteRenderer>();
	building2->LoadData(L"BackGround\\BG_Building.png");
	building2->SetDrawType(EDrawType::ScreenSpace);
	building2->SetRelativePosition(FVector2(width, 390));
	building2->m_layer = -5;

	auto m_building3 = GetWorld()->NewObject<gameObject>(L"Building3");
	//GetCamera()->AddChildObject(m_building);
	auto building3 = m_building3->AddComponent<SpriteRenderer>();
	building3->LoadData(L"BackGround\\BG_Building.png");
	building3->SetDrawType(EDrawType::ScreenSpace);
	building3->SetRelativePosition(FVector2(2 * width, 390));
	building3->m_layer = -5;

	auto m_building4 = GetWorld()->NewObject<gameObject>(L"Building4");
	//GetCamera()->AddChildObject(m_building);
	auto building4 = m_building4->AddComponent<SpriteRenderer>();
	building4->LoadData(L"BackGround\\BG_Building.png");
	building4->SetDrawType(EDrawType::ScreenSpace);
	building4->SetRelativePosition(FVector2(3 * width, 390));
	building->m_layer = -5;

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
	// 맨 뒷 숫자가 속도임
	//AddLooping(L"Building", L"BackGround\\BG_Building.png", -5, 340, 30);

	//AddLooping(L"GuardRail", L"BackGround\\BG_GuardRail.png", 12, 310, 450);
	//AddLooping(L"BackBarrier", L"BackGround\\BG_BackBarrier.png", 10, 250, 450);

	//AddLooping(L"Bridge", L"BackGround\\BG_Bridge.png", 4, 1190, 450);
	//AddLooping(L"Market", L"BackGround\\BG_Market.png", 5, 1140, 450);
	//AddLooping(L"FrontBarrier", L"BackGround\\BG_Barrier.png", 13, 740, 450);
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
