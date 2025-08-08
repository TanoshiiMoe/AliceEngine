#include "BikeStatScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/StatComponent.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Helpers/CoordHelper.h>
#include <Scripts/Weapon/BulletManager.h>
#include <Manager/TimerManager.h>
#include <Component/Collider.h>
#include <Manager/D2DRenderManager.h>

void BikeStatScript::Initialize()
{
	__super::Initialize();

	(void)StatTraits<BikeStatScript>::GetOffsetMap();

	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}


void BikeStatScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void BikeStatScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
	// 
	// 마우스 클릭 감지

	//if (Input::IsMouseLeftDown() && bCanFire)
	//{
	//	FVector2 ownerPos = owner->GetPosition();
	//	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
	//	BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, 300);
	//	bCanFire = false;
	//}
}

void BikeStatScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void BikeStatScript::Invoke()
{

}

void BikeStatScript::OnStart()
{
	//TimerManager::GetInstance().SetTimer(
	//	timer,
	//	[this]()
	//	{
	//		bCanFire = true;
	//		OutputDebugStringW(L"람다 타이머 호출됨!\n");
	//	},
	//	0.1f,
	//	true,
	//	0.0f
	//);

	// 여기에 OnStart에 대한 로직 작성
	//m_BikeStat = GetOwner();
	owner->AddComponent<SpriteRenderer>()->LoadData(L"BikeStatScript.png");
	BoxComponent* box = owner->AddComponent<BoxComponent>(owner->GetComponent<SpriteRenderer>()->GetRelativeSize(), FColor::Blue);
	box->SetSize(owner->GetComponent<SpriteRenderer>()->GetRelativeSize());
	box->SetIgnoreOwnerScale(false);

	/*
	* 게임오브젝트에 TextRenderComponent를 붙이는 예시
	*/

	TextRenderComponent* m_BikeStatTextCmp = owner->AddComponent<TextRenderComponent>();
	m_BikeStatTextCmp->SetText(owner->GetName());
	m_BikeStatTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_BikeStatTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_BikeStatTextCmp->SetFontSize(8);
	m_BikeStatTextCmp->SetColor(FColor::Gold);
	m_BikeStatTextCmp->SetRelativeScale(FVector2(3, 3));
	FVector2 widgetSize = m_BikeStatTextCmp->GetRelativeSize();
	m_BikeStatTextCmp->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 5.2f)));
	m_BikeNameTexts.push_back(m_BikeStatTextCmp);

	m_BikeStatTextCmp = owner->AddComponent<TextRenderComponent>();
	m_BikeStatTextCmp->SetText(L"직전체력");
	m_BikeStatTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_BikeStatTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_BikeStatTextCmp->SetRelativeScale(FVector2(3, 3));
	m_BikeStatTextCmp->SetFontSize(8);
	m_BikeStatTextCmp->SetColor(FColor::Green);
	m_BikeNameTexts.push_back(m_BikeStatTextCmp);

	m_BikeStatTextCmp = owner->AddComponent<TextRenderComponent>();
	m_BikeStatTextCmp->SetText(L"test");
	m_BikeStatTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_BikeStatTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_BikeStatTextCmp->SetRelativeScale(FVector2(3, 3));
	m_BikeStatTextCmp->SetColor(FColor::Green);
	m_BikeStatTextCmp->SetFontSize(8);
	m_BikeNameTexts.push_back(m_BikeStatTextCmp);

	m_BikeStatTextCmp = owner->AddComponent<TextRenderComponent>();
	m_BikeStatTextCmp->SetText(L"현재체력");
	m_BikeStatTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_BikeStatTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_BikeStatTextCmp->SetRelativeScale(FVector2(3, 3));
	m_BikeStatTextCmp->SetColor(FColor::Green);
	m_BikeStatTextCmp->SetFontSize(8);
	m_BikeNameTexts.push_back(m_BikeStatTextCmp);

	m_BikeStatTextCmp = owner->AddComponent<TextRenderComponent>();
	m_BikeStatTextCmp->SetText(L"test");
	m_BikeStatTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_BikeStatTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_BikeStatTextCmp->SetRelativeScale(FVector2(3, 3));
	m_BikeStatTextCmp->SetColor(FColor::Green);
	m_BikeStatTextCmp->SetFontSize(8);
	m_BikeNameTexts.push_back(m_BikeStatTextCmp);


	/*
	* 커스텀 구조체로 델리게이트를 바인딩 하는 예제
	*/
	m_bikeStat = owner->AddComponent<StatComponent<BikeStat>>();
	m_bikeStat->SetStat("HP", 30);
	m_bikeStat->SetStat("MAXHP", 30);
	m_bikeStat->SetStat("MP", 200);
	m_BikeNameTexts[1]->SetTextFormat(L"직전 체력 : ", m_bikeStat->GetStat("HP"));
	m_BikeNameTexts[2]->SetTextFormat(L"현재 체력 : ", m_bikeStat->GetStat("HP"));
	m_BikeNameTexts[3]->SetTextFormat(L"최대 체력 : ", m_bikeStat->GetStat("MAXHP"));
	m_BikeNameTexts[4]->SetTextFormat(L"배터리 : ", m_bikeStat->GetStat("BATTERY"));

	widgetSize = m_BikeNameTexts[1]->GetRelativeSize();
	m_BikeNameTexts[1]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 1.0)));
	widgetSize = m_BikeNameTexts[2]->GetRelativeSize();
	m_BikeNameTexts[2]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 2.0f)));
	widgetSize = m_BikeNameTexts[3]->GetRelativeSize();
	m_BikeNameTexts[3]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 3.0f)));
	widgetSize = m_BikeNameTexts[4]->GetRelativeSize();
	m_BikeNameTexts[4]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 0.0f)));

	m_bikeStat->OnChangeStatMap["HP"].Add(m_bikeStat->GetHandle(), [this](float oldVal, float newVal)
		{
			if (newVal <= 0)	// 죽는 시점
			{ 
				owner->RemoveComponent<BoxComponent>(owner->GetComponent<BoxComponent>());
				owner->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
				owner->RemoveComponent<Collider>(owner->GetComponent<Collider>());
				//owner->AddComponent<BoxComponent>(owner->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
				GetWorld()->RemoveObject(owner.Get());
				return;
			}
			//else if (oldVal <= 0)	// 부활하는 시점
			//{
				//owner->RemoveComponent<BoxComponent>(owner->GetComponent<BoxComponent>());
				//owner->GetComponent<SpriteRenderer>()->LoadData(L"BikeStatScript.png");
				//owner->AddComponent<BoxComponent>(owner->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
			//}
			m_BikeNameTexts[1]->SetTextFormat(L"직전 체력 : ", oldVal);
			m_BikeNameTexts[2]->SetTextFormat(L"현재 체력 : ", newVal);
		});;

	m_bikeStat->OnChangeStatMap["MAXHP"].Add(owner->GetHandle(), [this](float oldVal, float newVal)
		{
			m_BikeNameTexts[3]->SetTextFormat(L"최대 체력 : ", newVal);
		});;

	m_bikeStat->OnChangeStatMap["MP"].Add(owner->GetHandle(), [this](float oldVal, float newVal)
		{
			m_BikeNameTexts[4]->SetTextFormat(L"BATTERY : ", newVal);
		});;

	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });
}

void BikeStatScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	TimerManager::GetInstance().ClearTimer(timer);
}

void BikeStatScript::OnDestroy()
{
	m_BikeNameTexts.clear();
}

void BikeStatScript::Input()
{
	// 여기에 Input에 대한 로직 작성
}
