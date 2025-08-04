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
