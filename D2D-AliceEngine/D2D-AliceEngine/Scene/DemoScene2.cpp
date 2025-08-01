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
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� rawPointer�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene2::Initialize()
{
	__super::Initialize();
}

void DemoScene2::Release()
{
	__super::Release();
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
		L"\n"
		L" <ī�޶�> \n"
		L" [W,A,S,D]  : ī�޶� ��,��,��,�� �̵� \n"
		L" [1/2] : D2D, Unity ��ǥ�� \n"
		L" [ [ / ] ] : ī�޶� ���, Ȯ��\n"
		L" [Q] : ī�޶� �Ʒ翡�� ���̱� \n"
		L" [E] : ī�޶� ���� \n"
		L" * ī�޶� ���̸� ȭ��ǥ�� ī�޶� �̵��� �� �����ϴ�. \n"
		L"\n"
		L" <�Ʒ�> \n"
		L" [ȭ��ǥ ��/��/��/��] : ��,��,��,�� �̵� \n"
		L" [C] : �Ʒ� ���� \n"
		L" [B/N] : �Ʒ� ������ ��ȯ \n"
		L" [R] : �Ʒ� ȸ�� ��ȯ \n"
		L" [5,6] : ���� ����, ���� �ı� \n"
		L" [7,8] : ���� ����, ���� �ı� \n"
		L" [4] : �Ʒ� �̸� �ѿ� ��ȯ \n"
		L"\n"
		L" ::Delegate \n"
		L" [T] : �Ʒ簡 �Ʒ�2���� ������ �ֱ� \n"
		L" [Y] : �Ʒ� 5 ȸ���ϱ� \n"
		L" [U] : �Ʒ� �ִ�ü�� 10 �ø��� \n"
		L"\n"
		L" [G] : �Ʒ�2�� �Ʒ翡�� ������ �ֱ� \n"
		L" [H] : �Ʒ�2 7 ȸ���ϱ� \n"
		L" [J] : �Ʒ� �ִ�ü�� 15 �ø���"
	);
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 50));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(18.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));

	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(Define::SCREEN_WIDTH * 0.9, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	
	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_backgroundImage = NewObject<gameObject>(L"yuuka");
	m_backgroundImage->AddComponent<BackGroundImage>();

	//m_alice = NewObject<gameObject>(L"alice");
	//m_alice->transform()->SetPosition(-300, 0);
	//m_alice->transform()->SetRotation(0);
	//m_alice->transform()->SetScale(0.3f, 0.3f);
	//m_alice->transform()->SetPivot(0.5f);
	//m_alice->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Alice.webm", 30, L"jpg", 95, false);
	//m_alice->GetComponent<VideoComponent>()->Play();

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
	if (Input::IsKeyPressed(VK_C))
	{
		m_aruClone = Instantiate(m_aru);
	}

	if (Input::IsKeyDown(VK_B))
	{
		FVector2 scale = m_aru->transform()->GetScale() * 0.8f;
		m_aru->transform()->SetScale(scale.x, scale.y);
	}
	if (Input::IsKeyDown(VK_N))
	{
		FVector2 scale = m_aru->transform()->GetScale() * 1.3f;
		m_aru->transform()->SetScale(scale.x, scale.y);
	}
	if (Input::IsKeyDown(VK_R))
	{
		m_aru->transform()->SetRotation(m_aru->transform()->GetRotation() + 5.0f);
	}
}

void DemoScene2::CameraInput()
{
}
