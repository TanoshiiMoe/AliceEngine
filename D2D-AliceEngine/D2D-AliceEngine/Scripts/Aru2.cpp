#include "Aru2.h"
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

void Aru2::Initialize()
{
	__super::Initialize();
	(void)StatTraits<AruStat2>::GetOffsetMap();
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
}


void Aru2::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Aru2::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

}

void Aru2::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Aru2::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_aru2 = GetOwner();
	m_aru2->transform()->SetPosition(300, 0);
	m_aru2->transform()->SetRotation(0);
	m_aru2->transform()->SetScale(0.5f, 0.5f);
	m_aru2->transform()->SetPivot(0.5f);
	m_aru2->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Green);

	/*
	* ���ӿ�����Ʈ�� TextRenderComponent�� ���̴� ����
	*/

	TextRenderComponent* m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(m_aru2->GetName());
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_Aru2TextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.5f));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_Aru2TextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.2f));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_Aru2TextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.3f));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_Aru2TextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.4f));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_Aru2TextCmp->SetPosition(FVector2(0, -m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize().y * 0.1f));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);


	/*
	* Ŀ���� ����ü�� ��������Ʈ�� ���ε� �ϴ� ����
	*/
	m_aru2Stat = m_aru2->AddComponent<StatComponent<AruStat2>>();
	m_aru2Stat->SetStat("HP", 100);
	m_aru2Stat->SetStat("MAXHP", 100);
	m_aru2Stat->SetStat("MP", 150);
	m_aru2NameTexts[1]->SetTextFormat(L"���� ü�� : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[2]->SetTextFormat(L"���� ü�� : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", m_aru2Stat->GetStat("MAXHP"));
	m_aru2NameTexts[4]->SetTextFormat(L"���� : ", m_aru2Stat->GetStat("MP"));

	m_aru2Stat->OnChangeStatMap["HP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		if (newVal <= 0)	// �״� ����
		{
			m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
			m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
			m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
		}
		else if (oldVal <= 0)	// ��Ȱ�ϴ� ����
		{
			m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
			m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
			m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
		}
		m_aru2NameTexts[1]->SetTextFormat(L"���� ü�� : ", oldVal);
		m_aru2NameTexts[2]->SetTextFormat(L"���� ü�� : ", newVal);
	});;

	m_aru2Stat->OnChangeStatMap["MAXHP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aru2NameTexts[3]->SetTextFormat(L"�ִ� ü�� : ", newVal);
	});;

	m_aru2Stat->OnChangeStatMap["MP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aru2NameTexts[4]->SetTextFormat(L"���� : ", newVal);
	});;
}

void Aru2::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Aru2::OnDestroy()
{
	m_aru2NameTexts.clear();
}

void Aru2::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
