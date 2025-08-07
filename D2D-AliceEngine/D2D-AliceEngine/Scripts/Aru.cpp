#include "Aru.h"
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

void Aru::Initialize()
{
	__super::Initialize();
	(void)StatTraits<AruStat>::GetOffsetMap();
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}


void Aru::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Aru::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�
	// 
	// ���콺 Ŭ�� ����

	if (Input::IsMouseLeftDown() && bCanFire)
	{
		FVector2 ownerPos = owner->GetPosition();
		FVector2 worldMousePos = Input::GetMouseWorldPosition(); // ���콺�� ���� ���� ��ǥ
		BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, 300);
		bCanFire = false;
	}
}

void Aru::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Aru::Invoke()
{

}

void Aru::OnStart()
{
	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bCanFire = true;
			OutputDebugStringW(L"���� Ÿ�̸� ȣ���!\n");
		}, 
		0.1f,
		true,
		0.0f
	);

	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_aru = GetOwner();
	m_aru->transform()->SetPosition(0, 0);
	m_aru->transform()->SetRotation(0);
	m_aru->transform()->SetScale(0.25f, 0.25f);
	m_aru->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetRelativeSize(), FColor::Blue);
	m_aru->GetComponent<BoxComponent>()->SetSize(m_aru->GetComponent<SpriteRenderer>()->GetRelativeSize());
	m_aru->GetComponent<BoxComponent>()->SetIgnoreOwnerScale(false);

	/*
	* ���ӿ�����Ʈ�� TextRenderComponent�� ���̴� ����
	*/

	TextRenderComponent* m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(m_aru->GetName());
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetFontSize(24);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	FVector2 widgetSize = m_aruTextCmp->GetRelativeSize();
	m_aruTextCmp->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 5.2f)));
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);

	m_aruTextCmp = m_aru->AddComponent<TextRenderComponent>();
	m_aruTextCmp->SetText(L"test");
	m_aruTextCmp->SetDrawType(EDrawType::WorldSpace);
	m_aruTextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_aruTextCmp->SetRelativeScale(FVector2(3, 3));
	m_aruTextCmp->SetFontSize(24);
	m_aruNameTexts.push_back(m_aruTextCmp);


	/*
	* Ŀ���� ����ü�� ��������Ʈ�� ���ε� �ϴ� ����
	*/
	m_aruStat = m_aru->AddComponent<StatComponent<AruStat>>();
	m_aruStat->SetStat("HP", 30);
	m_aruStat->SetStat("MAXHP", 30);
	m_aruStat->SetStat("MP", 200);
	m_aruNameTexts[1]->SetTextFormat(L"���� ü�� : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[2]->SetTextFormat(L"���� ü�� : ", m_aruStat->GetStat("HP"));
	m_aruNameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", m_aruStat->GetStat("MAXHP"));
	m_aruNameTexts[4]->SetTextFormat(L"���� : ", m_aruStat->GetStat("MP"));

	widgetSize = m_aruNameTexts[1]->GetRelativeSize();
	m_aruNameTexts[1]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 1.0)));
	widgetSize = m_aruNameTexts[2]->GetRelativeSize();
	m_aruNameTexts[2]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 2.0f)));
	widgetSize = m_aruNameTexts[3]->GetRelativeSize();
	m_aruNameTexts[3]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 3.0f)));
	widgetSize = m_aruNameTexts[4]->GetRelativeSize();
	m_aruNameTexts[4]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 0.0f)));

	m_aruStat->OnChangeStatMap["HP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		if (newVal <= 0)	// �״� ����
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
		}
		else if (oldVal <= 0)	// ��Ȱ�ϴ� ����
		{
			m_aru->RemoveComponent<BoxComponent>(m_aru->GetComponent<BoxComponent>());
			m_aru->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
			m_aru->AddComponent<BoxComponent>(m_aru->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
		}
		m_aruNameTexts[1]->SetTextFormat(L"���� ü�� : ", oldVal);
		m_aruNameTexts[2]->SetTextFormat(L"���� ü�� : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MAXHP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aruNameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", newVal);
	});;

	m_aruStat->OnChangeStatMap["MP"].Add(m_aru->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aruNameTexts[4]->SetTextFormat(L"���� : ", newVal);
	});;

	m_aru->AddComponent<InputComponent>()->SetAction(m_aru->GetHandle(), [this]() { Input(); });
}

void Aru::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
	TimerManager::GetInstance().ClearTimer(timer);
}

void Aru::OnDestroy()
{
	m_aruNameTexts.clear();
}

void Aru::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�

	if (Input::IsKeyPressed(VK_4))
	{
		if (m_aru->GetComponent<TextRenderComponent>()->GetText() == L"aru")
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"�Ʒ�");
		else
			m_aru->GetComponent<TextRenderComponent>()->SetText(L"aru");
	}
	if (Input::IsKeyPressed(VK_9))
	{
		//for (int i = 0; i < 5000; i++)
		//{
		//	GetWorld()->RemoveObjectByName(L"gun");
		//}
		GetWorld()->RemoveAllObjectsByName(L"gun");
	}
	if (Input::IsKeyPressed(VK_0))
	{
		for (int i = 0; i < 5000; i++)
		{
			gameObject* gun = GetWorld()->NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.1f, 0.5f), FRandom::GetRandomInRange(0.1f, 0.5f)), FVector2(0.5f));
			gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
		}
	}
	if (Input::IsKeyDown(VK_5))
	{
		// �����ڷ� ���� ���� ����
		gameObject* gun = GetWorld()->NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");

		// �ּ��� ���ô� transform�� �ϳ��� set�ϴ� ���
		//gameObject* gun = NewObject<gameObject>(L"gun", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		//gun->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512));
		//gun->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//gun->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//gun->transform()->SetPivot(0.5f);
		//gun->AddComponent<SpriteRenderer>()->LoadData(L"Gun.png");
	}
	if (Input::IsKeyDown(VK_6))
	{
		GetWorld()->RemoveObjectByName(L"gun");
	}
	if (Input::IsKeyDown(VK_7))
	{
		gameObject* wallet = GetWorld()->NewObject<gameObject>(L"wallet", FVector2(FRandom::GetRandomInRange(-512, 512), FRandom::GetRandomInRange(-512, 512)), FRandom::GetRandomInRange(0, 90), FVector2(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f)), FVector2(0.5f));
		wallet->AddComponent<SpriteRenderer>()->LoadData(L"wallet.png");
		//wallet->transform()->SetPosition(FRandom::GetRandomInRange(-512, 512));
		//wallet->transform()->SetRotation(FRandom::GetRandomInRange(0, 90));
		//wallet->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.8f), FRandom::GetRandomInRange(0.3f, 0.8f));
		//wallet->transform()->SetPivot(0.5f);
	}
	if (Input::IsKeyDown(VK_8))
	{
		GetWorld()->RemoveObjectByName(L"wallet");
	}
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"SolarSystemScene");
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_aru->GetComponent<SpriteRenderer>()->SetFlip(true);
		m_aru->transform()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_aru->GetComponent<SpriteRenderer>()->SetFlip(false);
		m_aru->transform()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		m_aru->transform()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		m_aru->transform()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_K))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.width += 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.x += 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.height += 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.y += 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_O))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.width -= 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.x -= 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
	if (Input::IsKeyDown(VK_P))
	{
		m_aru->GetComponent<SpriteRenderer>()->spriteInfo.height -= 10;
		/*FVector2 scale = m_aru->GetComponent<SpriteRenderer>()->GetScale();
		scale.y -= 0.1f;
		m_aru->GetComponent<SpriteRenderer>()->SetScale(scale);*/
	}
}
