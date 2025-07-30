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
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� rawPointer�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene3::Initialize()
{
	__super::Initialize();
}

void DemoScene3::Release()
{
	__super::Release();
}

void DemoScene3::Update()
{
	__super::Update();
	Define::ERigidBodyType rigidType = Define::ERigidBodyType::Max;
	std::wstring rigidTypeStr = L"";
	if (m_player->GetComponent<Rigidbody2D>())
	{
		switch (rigidType)
		{
		case Define::ERigidBodyType::Static:
			rigidTypeStr = L"Static";
			break;
		case Define::ERigidBodyType::Kinematic:
			rigidTypeStr = L"Kinematic";
			break;
		case Define::ERigidBodyType::Dynamic:
			rigidTypeStr = L"Dynamic";
			break;
		default:
			rigidTypeStr = L"Unknown";
			break;
		}
	}

	FVector2 pos = m_player->transform()->GetPosition();
	float rotation = m_player->transform()->GetRotation();
	FVector2 scale = m_player->transform()->GetScale();
	std::wstring playerMovement = L"nullptr";
	if (m_player->GetComponent<Player>())
	{
		playerMovement = m_player->GetComponent<Player>()->bMoveRigidBody ? L"RigidBody AddForce" : L"Transform SetTransform";
	}
	std::wstring playerSpeed = L"nullptr";
	if (m_player->GetComponent<Player>())
	{
		playerSpeed = std::to_wstring(m_player->GetComponent<Player>()->walkSpeed);
	}
	std::wstring playerGravity = L"nullptr";
	if (m_player->GetComponent<Rigidbody2D>())
	{
		playerGravity = std::to_wstring(m_player->GetComponent<Rigidbody2D>()->gravityScale);
	}
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
		L"\n"
		L" player movement : ("
		+ playerMovement
		+ L")"
		L"\n"
		L" player Speed : ("
		+ playerSpeed
		+ L")"
		L"\n"
		L" player Gravity : ( -9.8 * "
		+ playerGravity
		+ L")"
		L"\n"
		L" Rigidbody Type : "
		+ rigidTypeStr
	);
	for (int i = 0; i < enemyMax; i++)
	{
		if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
		{
			if(m_enemies[i]->GetComponent<Collider>())
				m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Green);
		}
		else if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody)
		{
			if(m_enemies[i]->GetComponent<Collider>())
				m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Blue);
		}
		else
		{
			if(m_enemies[i]->GetComponent<Collider>())
				m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Red);
		}
	}
	if (m_player->GetComponent<Rigidbody2D>())
	{
		if (m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
		{
			if(m_player->GetComponent<Collider>())
				m_player->GetComponent<Collider>()->SetBoxColor(FColor::Green);
		}
		else
		{
			if(m_player->GetComponent<Collider>())
				m_player->GetComponent<Collider>()->SetBoxColor(FColor::Red);
		}
	}
}

void DemoScene3::OnEnter()
{
	__super::OnEnter();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget4 = NewObject<gameObject>(L"widget4");

	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <�÷��̾�> \n"
		L" [ȭ��ǥ ��,��] : �÷��̾� ��,�Ʒ� �̵�\n"
		L" [ȭ��ǥ ��/��] : �÷��̾� ��,�� �̵� \n"
		L" [z] : ���� \n"
		L" [SpaceBar] : ���� (2�� �������� ����) \n"
		L" [k] : �÷��̾� �ӵ� ���� - �ͳθ� �˻� \n"
		L" [l] : �÷��̾� �ӵ� ����\n"
		L" [g] : �÷��̾� �߷� ���� - �ͳθ� �˻� \n"
		L" [h] : �÷��̾� �߷� ����\n"
		L" [u] : �÷��̾� �̵� ��� ����\n"
		L" [i] : Rigidbody�� Kinematic���� ��ȯ\n"
		L" [o] : Rigidbody�� Dynamic���� ��ȯ\n"
		L" [p] : Rigidbody�� Static���� ��ȯ\n"
		L" [t] : Rigidbody2D ����\n"
		L" [y] : Rigidbody2D ����\n"
		L" [8] : Collider ����\n"
		L" [9] : Collider ����\n"
		L"\n"
		L" <�÷��̾� ����>\n"
		L" Box ���� : �ʷϻ� - �� ���� ����\n"
		L" Box ���� : �Ķ���/����� - �� ���� �ִ� ��ü ���� ����\n"
		L" Box ���� : ������ - ���� �߿� ����\n"
		L" Rigidbody Ÿ�Կ� ���� ���� �ùķ��̼� ����/�������� �޶����ϴ�.\n"
		L" - Static : �������� ����(����/�� ��)\n"
		L" - Kinematic : ���� ����, ���� �� ����\n"
		L" - Dynamic : �߷�/�� �� ���� ����\n"
		L" [1/2] : D2D, Unity ��ǥ�� \n"
		L" [Q] : ī�޶� �÷��̾�� ���̱� \n"
		L" [E] : ī�޶� ���� \n"
		L" [R] : Scene ����� \n"
		L"\n"
	);	
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(18, 45));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(18.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));
	m_widget->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget2->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget3->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_widget4->transform()->SetPosition(512, 512);
	m_widget4->AddComponent<TextRenderComponent>()->SetText(L" test");
	m_widget4->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget4->GetComponent<TextRenderComponent>()->SetPosition(FVector2(45, 60));
	m_widget4->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget4->GetComponent<TextRenderComponent>()->m_layer = 20;

	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });

	for (int i = 0; i < enemyMax; i++)
	{
		m_enemies.push_back(NewObject<gameObject>(L"enemy"));
		m_enemies[i]->AddComponent<Enemy>();
		m_enemies[i]->AddComponent<InputComponent>()->SetAction(m_enemies[i]->GetHandle(), [this]() { EnemyInput(); });

		// �÷��̾�� ���� ������ ��ġ���� �����ǰ��ϴ� ����
		/*m_enemies[i]->transform()->SetPosition(
			FRandom::GetRandomPointInTorus2D(
				m_player->transform()->GetPosition().x,
				m_player->transform()->GetPosition().y,
				200.0f,
				412.0f));*/
				// Ư�� �������� �� ����̳��� ������ ��ġ���� �����ǰ��ϴ� ����
		m_enemies[i]->transform()->SetPosition(
			FRandom::GetRandomPointInCircle2D(
				300,
				300,
				200.0f
			));

		m_enemies[i]->transform()->SetRotation(0);
		m_enemies[i]->transform()->SetScale(FRandom::GetRandomInRange(0.3f, 0.9f));
		m_enemies[i]->transform()->SetPivot(0.5f);

		//m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(50, 80));
		m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(FRandom::GetRandomInRange(10, 50), FRandom::GetRandomInRange(30, 100)));
		m_enemies[i]->AddComponent<Rigidbody2D>();
		m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
		m_enemies[i]->GetComponent<Rigidbody2D>()->gravityScale = 6;
		m_enemies[i]->GetComponent<Rigidbody2D>()->mass = 10;
	}

	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));

	m_wall2 = NewObject<gameObject>(L"wall2");
	m_wall2->transform()->SetPivot(0.5f);
	m_wall2->transform()->SetPosition(-400, 0);
	m_wall2->AddComponent<Collider>()->SetBoxSize(FVector2(60, 5500));

	m_wall2 = NewObject<gameObject>(L"wall2");
	m_wall2->transform()->SetPivot(0.5f);
	m_wall2->transform()->SetPosition(700, 0);
	m_wall2->AddComponent<Collider>()->SetBoxSize(FVector2(60, 5500));
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
	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"aruScene");
	}
	if (Input::IsKeyPressed(VK_R))
	{
		SceneManager::RestartScene();
	}
}