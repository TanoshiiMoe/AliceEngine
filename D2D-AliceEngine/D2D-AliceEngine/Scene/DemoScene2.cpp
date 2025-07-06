#include "DemoScene2.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Aru.h"
#include "../Scripts/Aru2.h"
#include "../Scripts/Player.h"
#include "../Scripts/CameraController.h"

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene2::Initialize()
{
}

void DemoScene2::Release()
{
}

void DemoScene2::Update()
{
	__super::Update();
}

void DemoScene2::OnEnter()
{
	__super::OnEnter();

	m_camera = NewObject<gameObject>(L"Camera");
	m_camera->AddComponent<CameraController>();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");

	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <카메라> \n"
		L" [화살표 상,하] : 카메라 위,아래 이동 \n"
		L" [화살표 좌/우] : 카메라 좌,우 이동 \n"
		L" [1/2] : D2D, Unity 좌표계 \n"
		L" [[/]] : 카메라 축소, 확대\n"
		L" [Q] : 카메라를 아루에게 붙이기 \n"
		L" [E] : 카메라를 떼기 \n"
		L" * 카메라를 붙이면 화살표로 카메라를 이동할 수 없습니다. \n"
		L"\n"
		L" <배경 실행> \n"
		L" [P] : 배경 실행, 중지 \n"
		L"\n"
		L" <아루> \n"
		L" [5,6] : 무기 스폰, 무기 파괴 \n"
		L" [7,8] : 지갑 스폰, 지갑 파괴 \n"
		L" [W,A,S,D] : 이동 \n"
		L" [4] : 아루 이름 한영 전환 \n"
		L" ::Delegate \n"
		L" [T] : 아루가 아루2에게 데미지 주기 \n"
		L" [Y] : 아루 5 회복하기 \n"
		L" [U] : 아루 최대체력 10 늘리기 \n"
		L" [G] : 아루2가 아루에게 데미지 주기 \n"
		L" [H] : 아루2 7 회복하기 \n"
		L" [J] : 아루 최대체력 15 늘리기"
	);
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 50));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));

	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <씬> \n [3] : 씬 전환");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	
	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <현재 씬> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_backgroundImage = NewObject<gameObject>(L"yuuka");
	m_backgroundImage->AddComponent<BackGroundImage>();

	m_aru = NewObject<gameObject>(L"aru");
	m_aru->AddComponent<Aru>();

	m_aru2 = NewObject<gameObject>(L"aru2");
	m_aru2->AddComponent<Aru2>();
	
	m_aru->AddComponent<InputComponent>()->SetAction(m_aru->GetHandle(), [this]() { aruInput(); });
	m_aru2->AddComponent<InputComponent>()->SetAction(m_aru2->GetHandle(), [this]() { aru2Input(); });
}

void DemoScene2::OnExit()
{
	__super::OnExit();
}

void DemoScene2::aru2Input()
{
	if (Input::IsKeyPressed(VK_G))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->DecreaseAbility("HP", 5);
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->DecreaseAbility("MP", 4);
	}
	if (Input::IsKeyPressed(VK_H))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->IncreaseAbility("MAXHP", "HP", 7);
	}
	if (Input::IsKeyPressed(VK_J))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->SetStat("MAXHP", m_aru2->GetComponent<Aru2>()->m_aru2Stat->GetStat("MAXHP") + 15);
	}
}

void DemoScene2::aruInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_aru);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}
	if (Input::IsKeyPressed(VK_T))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->DecreaseAbility("HP", 10);
		m_aru->GetComponent<Aru>()->m_aruStat->DecreaseAbility("MP", 5);
	}
	if (Input::IsKeyPressed(VK_Y))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->IncreaseAbility("MAXHP", "HP", 5);
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->SetStat("MAXHP", m_aru->GetComponent<Aru>()->m_aruStat->GetStat("MAXHP")+10);
	}
}