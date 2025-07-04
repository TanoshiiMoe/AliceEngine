#include "DemoScene3.h"
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
#include "../Scripts/Enemy.h"

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene3::Initialize()
{
}

void DemoScene3::Release()
{
}

void DemoScene3::Update()
{
	__super::Update();
	FVector2 pos = m_player->transform()->GetPosition();
	float rotation = m_player->transform()->GetRotation();
	FVector2 scale = m_player->transform()->GetScale();
	m_widget4->GetComponent<TextRenderComponent>()->SetText(
		L" [info] \n"
		L" player Position : ("
		+ std::to_wstring(pos.x)
		+ L", "
		+ std::to_wstring(pos.y)
		+ L")"
		L"\n"
		L" player Rotation : ("
		+ std::to_wstring(rotation)
		+ L")"
		L"\n"
		L" player Scale : ("
		+ std::to_wstring(scale.x)
		+ L", "
		+ std::to_wstring(scale.y)
		+ L")"
	);
}

void DemoScene3::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget4 = NewObject<gameObject>(L"widget4");

	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <플레이어> \n"
		L" [화살표 상,하] : 플레이어 위,아래 이동 \n"
		L" [화살표 좌/우] : 플레이어 좌,우 이동 \n"
		L" [z] : 공격 \n"
		L"\n"
		L"\n"
		L" [1/2] : D2D, Unity 좌표계 \n"
		L" [Q] : 카메라를 플레이어에게 붙이기 \n"
		L" [E] : 카메라를 떼기 \n"
		L"\n"
	);	
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 100));
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

	m_widget4->transform()->SetPosition(0, 0);
	m_widget4->AddComponent<TextRenderComponent>()->SetText(L" test");
	m_widget4->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::MiddleCenter);
	m_widget4->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 600));
	m_widget4->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player, [this]() { PlayerInput(); });

	for (int i = 0; i < enemyMax; i++)
	{
		m_enemies.push_back(NewObject<gameObject>(L"enemy"));
		m_enemies[i]->AddComponent<Enemy>();
		m_enemies[i]->AddComponent<InputComponent>()->SetAction(m_enemies[i], [this]() { EnemyInput(); });

		m_enemies[i]->transform()->SetPosition(
			FRandom::GetRandomPointInTorus2D(
				m_player->transform()->GetPosition().x,
				m_player->transform()->GetPosition().y,
				300.0f,
				512.0f));
		m_enemies[i]->transform()->SetRotation(0);
		m_enemies[i]->transform()->SetScale(FRandom::GetRandomInRange(0.3, 0.9f));
		m_enemies[i]->transform()->SetPivot(0.5f);
	}
}

void DemoScene3::OnExit()
{
	__super::OnExit();
	m_enemies.clear();
}

void DemoScene3::EnemyInput()
{
}

void DemoScene3::PlayerInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_player);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
}