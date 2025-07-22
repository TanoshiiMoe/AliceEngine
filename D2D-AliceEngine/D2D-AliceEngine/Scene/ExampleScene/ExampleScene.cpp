#include "ExampleScene.h"
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
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>

// �� ���� ���� ���������� ������ ��ũ��Ʈ ������Ʈ���Դϴ�.
#include "../../Scripts/BackGroundImage.h"
#include "../../Scripts/Aru.h"
#include "../../Scripts/Aru2.h"
#include "../../Scripts/Player.h"
#include "../../Scripts/Enemy.h"

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� rawPointer�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*
*   Update() �Լ��� if������ ��ǲ�� ���� �� �� �ֽ��ϴ�. Ȥ�� Enter() ���� InputComponent�� SetAction���� ��ǲ�� ����� �� �ֽ��ϴ�.
*
*/

void ExampleScene::Initialize()
{
	__super::Initialize();
}

void ExampleScene::Release()
{
	__super::Release();
}

void ExampleScene::Update()
{
	__super::Update();
	// player�� position, rotation, scale�� �������� ���
	FVector2 pos = m_player->transform()->GetPosition();
	float rotation = m_player->transform()->GetRotation();
	FVector2 scale = m_player->transform()->GetScale();

	// for������ ���鼭 �����ϴ� ���
	// ���� �ִ¸� Collider�� ���� �ʷϻ�����, �ƴϸ� ���������� �ٲٴ� ����
	for (int i = 0; i < enemyMax; i++)
	{
		if (m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
		{
			m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Green);
		}
		else
		{
			m_enemies[i]->GetComponent<Collider>()->SetBoxColor(FColor::Red);
		}
	}
	if (m_player->GetComponent<Rigidbody2D>()->m_eRigidBodyState == Define::ERigidBodyState::Ground)
	{
		m_player->GetComponent<Collider>()->SetBoxColor(FColor::Green);
	}
	else
	{
		m_player->GetComponent<Collider>()->SetBoxColor(FColor::Red);
	}
}

void ExampleScene::OnEnter()
{
	__super::OnEnter();

	// ������ �����ϰ� ������Ʈ�� �����ϴ� �����Դϴ�.
	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <�÷��̾�> \n"
		L" ������ ������ �˴ϴ�.\n"
		L" �ؽ�Ʈ �ؽ�Ʈ \n"
		L" �ؽ�Ʈ �ؽ�Ʈ �ؽ�Ʈ \n"
		L"\n"
	);
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(18, 60));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));
	m_widget->GetComponent<TextRenderComponent>()->m_layer = 20;

	// ��׶��� ��� �Դϴ�.
	m_backgroundImage = NewObject<gameObject>(L"sky");
	m_backgroundImage->AddComponent<BackGroundImage>();

	// �÷��̾� �����Դϴ�.
	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<Player>();
	m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput(); });


	// �� �����Դϴ�. ���� vector�� ��� ó���մϴ�.
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
		m_enemies[i]->transform()->SetScale(FRandom::GetRandomInRange(0.3, 0.9f));
		m_enemies[i]->transform()->SetPivot(0.5f);

		m_enemies[i]->AddComponent<Collider>()->SetBoxSize(FVector2(FRandom::GetRandomInRange(10, 50), FRandom::GetRandomInRange(30, 100)));
		m_enemies[i]->AddComponent<Rigidbody2D>();
		m_enemies[i]->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
		m_enemies[i]->GetComponent<Rigidbody2D>()->gravityScale = 6;
		m_enemies[i]->GetComponent<Rigidbody2D>()->mass = 10;
	}

	// �� �����Դϴ�.
	m_wall = NewObject<gameObject>(L"wall");
	m_wall->transform()->SetPivot(0.5f);
	m_wall->transform()->SetPosition(0, -300);
	m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));
}

void ExampleScene::OnExit()
{
	__super::OnExit();
	m_enemies.clear();
}

void ExampleScene::EnemyInput()
{
}

void ExampleScene::PlayerInput()
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
	if (Input::IsKeyPressed(VK_R))
	{
		SceneManager::RestartScene();
	}
	if (Input::IsKeyDown(VK_K))
	{
		m_player->GetComponent<Player>()->walkSpeed += 5;
	}
	if (Input::IsKeyDown(VK_L))
	{
		m_player->GetComponent<Player>()->walkSpeed -= 5;
	}
	if (Input::IsKeyDown(VK_G))
	{
		m_player->GetComponent<Rigidbody2D>()->gravityScale += 0.1;
	}
	if (Input::IsKeyDown(VK_H))
	{
		m_player->GetComponent<Rigidbody2D>()->gravityScale -= 0.1;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_player->GetComponent<Player>()->bMoveRigidBody = !m_player->GetComponent<Player>()->bMoveRigidBody;
	}
}