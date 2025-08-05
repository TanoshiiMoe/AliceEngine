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
#include <Component/BackGroundComponent.h>

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
	//	if (pos2.x <= -restX)
	//		pos2.x = pos1.x + restX;

	//	layer.obj1->transform()->SetPosition(pos1);
	//	layer.obj2->transform()->SetPosition(pos2);
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

	m_sky = GetWorld()->NewObject<gameObject>(L"Sky");

	auto sky = m_sky->AddComponent<SpriteRenderer>();
	sky->LoadData(L"BackGround\\BG_Sky.png");
	sky->SetDrawType(EDrawType::ScreenSpace);
	sky->SetRelativePosition(FVector2(960,550));
	sky->m_layer = -10;

	//auto obj1 = GetWorld()->NewObject<gameObject>(L"Building_1");
	//auto* sr1 = obj1->AddComponent<SpriteRenderer>();
	//sr1->LoadData(L"BackGround\\BG_Building.png");
	//sr1->SetRelativePosition(FVector2(sr1->GetBitmapSizeX() / 2.0f, 200));
	//sr1->m_layer = -5;

	//auto obj2 = GetWorld()->NewObject<gameObject>(L"Building_2");
	//auto* sr2 = obj2->AddComponent<SpriteRenderer>();
	//sr2->LoadData(L"BackGround\\BG_Building.png");
	//sr2->SetRelativePosition(FVector2(sr2->GetBitmapSizeX() / 2.0f, 200));
	//sr2->m_layer = -5;

	//float width = sr1->GetBitmapSizeX();
	//float y = 200;
	//
	//sr1->SetRelativePosition(FVector2(0, y));
	//sr2->SetRelativePosition(FVector2(width, y));

	//m_loopingLayers.push_back({ obj1, obj2, 1.0f, width, y });

	m_building = GetWorld()->NewObject<gameObject>(L"Building");
	auto building = m_building->AddComponent<SpriteRenderer>();
	building->LoadData(L"BackGround\\BG_Building.png");
	building->SetRelativePosition(FVector2(building->GetBitmapSizeX() / 2.0f, 200));
	building->m_layer = -5;

	m_bridge = GetWorld()->NewObject<gameObject>(L"Bridge");
	auto bridge = m_bridge->AddComponent<SpriteRenderer>();
	bridge->LoadData(L"BackGround\\BG_Bridge.png");
	bridge->SetRelativePosition(FVector2(bridge->GetBitmapSizeX() / 2.0f, -650));
	bridge->m_layer = 4;

	m_market = GetWorld()->NewObject<gameObject>(L"Market");
	auto market = m_market->AddComponent<SpriteRenderer>();
	market->LoadData(L"BackGround\\BG_Market.png");
	market->SetRelativePosition(FVector2(market->GetBitmapSizeX() / 2.0f, -600));
	market->m_layer = 5;

	m_backBarrier = GetWorld()->NewObject<gameObject>(L"BackBarrier");
	auto backBarrier = m_backBarrier->AddComponent<SpriteRenderer>();
	backBarrier->LoadData(L"BackGround\\BG_Barrier.png");
	backBarrier->SetRelativePosition(FVector2(backBarrier->GetBitmapSizeX() / 2.0f, 250));
	backBarrier->m_layer = 10;

	m_frontBarrier = GetWorld()->NewObject<gameObject>(L"FrontBarrier");
	auto frontBarrier = m_frontBarrier->AddComponent<SpriteRenderer>();
	frontBarrier->LoadData(L"BackGround\\BG_Barrier.png");
	frontBarrier->SetRelativePosition(FVector2(frontBarrier->GetBitmapSizeX() / 2.0f, -200));
	frontBarrier->m_layer = 11;

	m_guardrail = GetWorld()->NewObject<gameObject>(L"GuardRail");
	auto guardrail = m_guardrail->AddComponent<SpriteRenderer>();
	guardrail->LoadData(L"BackGround\\BG_GuardRail.png");
	guardrail->SetRelativePosition(FVector2(guardrail->GetBitmapSizeX() / 2.0f, 240));
	guardrail->m_layer = 12;
}

void BackGroundRender::OnEnd()
{
}

void BackGroundRender::OnDestroy()
{
}
