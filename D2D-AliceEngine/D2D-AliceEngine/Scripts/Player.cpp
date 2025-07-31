#include "Player.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/RenderComponent.h>
#include <Component/Animator.h>
#include <System/ScriptSystem.h>

#include <Animation/TextureLoader.h>
#include <Animation/AnimationController.h>
#include <Animation/AnimatorInstance.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include "Effect/Prism.h"
#include <Manager/SceneManager.h>

#include <Component/BackGroundComponent.h>
#include <Component/SkewTransform.h>

void Player::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Player::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Player::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_owner);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}

	float speed = walkSpeed * deltaSeconds;
	//float speed = 125.0f;
	if (!(Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_DOWN) || Input::IsKeyDown(VK_UP)))
	{
		animInstance->SetFloat("speed", 0);
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(speed, 0);
		else
			m_owner->transform()->AddPosition(speed, 0);
		animInstance->SetFlip(true);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(-speed, 0);
		else
			m_owner->transform()->AddPosition(-speed, 0);
		animInstance->SetFlip(false);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, -speed);
		else
			m_owner->GetComponent<SkewTransform>()->zPos -= speed;
			//m_owner->transform()->AddPosition(0, -speed);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, speed);
		else
			m_owner->GetComponent<SkewTransform>()->zPos += speed;
			//m_owner->transform()->AddPosition(0, speed);
	}
	// ���� ī��Ʈ ����: ���� ������ jumpCount = 0
	auto rb = m_owner->GetComponent<Rigidbody2D>();
	if (rb)
	{
		bool isGround = (rb->m_eRigidBodyState == Define::ERigidBodyState::Ground ||
			rb->m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody);
		if (isGround && prevGroundState == 0)
		{
			jumpCount = 0; // ���� ó�� ����� ���� ����
		}
		prevGroundState = isGround ? 1 : 0;
	}


	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
}

void Player::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Player::Awake()
{
}

void Player::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();

	m_owner->transform()->SetPosition(0, 0);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(1.5f, 1.5f);
	m_owner->transform()->SetPivot(0.5f);
	
	AnimatorController::LoadAnimatorController(L"Zero/Zero_AnimController.json", animController);
	animInstance = m_owner->AddComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Zero\\Zero_sprites.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_idle_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_attack_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_walk_anim.json");
	animInstance->ApplyClipDurationsToStates();
	//animInstance->SetLooping(true);
	animInstance->Play();
	animInstance->m_layer = 5;
	animInstance->OnStart();

	m_owner->AddComponent<Collider>()->SetBoxSize(FVector2(35,60));
	m_owner->AddComponent<Rigidbody2D>();
	if (auto rb = m_owner->GetComponent<Rigidbody2D>())
	{
		rb->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
		rb->gravityScale = 60.0f;
		rb->mass = 20.0f;
		rb->drag = 0.6f;
		//rb->angularDrag = 0;
		bMoveRigidBody = false;
	}

	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });

	/*m_background->AddComponent<BackGroundComponent>();
	m_background->transform()->SetPosition(m_owner->transform()->GetPosition().x, Define::SCREEN_HEIGHT);*/

	// �굥��ź �׽�Ʈ
	m_owner->AddComponent<Prism>(10, 0.1f);
}

void Player::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
	//m_owner->GetComponent<Collider>()->SetBoxSize(FVector2(55, 90));
}

void Player::OnDestroy()
{
}

void Player::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�

	if (Input::IsKeyDown(VK_K))
	{
		walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		walkSpeed -= 5.0f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		bMoveRigidBody = !bMoveRigidBody;
	}
	if (Input::IsKeyDown(VK_K))
	{
		walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		walkSpeed -= 5.0f;
	}
	if (Input::IsKeyDown(VK_G))
	{
		m_owner->GetComponent<Rigidbody2D>()->gravityScale += 0.1f;
	}
	if (Input::IsKeyDown(VK_H))
	{
		m_owner->GetComponent<Rigidbody2D>()->gravityScale -= 0.1f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		bMoveRigidBody = !bMoveRigidBody;
	}
	if (Input::IsKeyPressed(VK_I))
	{
		m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
	}
	if (Input::IsKeyPressed(VK_O))
	{
		m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
	}
	if (Input::IsKeyPressed(VK_P))
	{
		m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Static;
	}

	if (Input::IsKeyDown(VK_T))
	{
		m_owner->RemoveComponent<Rigidbody2D>(m_owner->GetComponent<Rigidbody2D>());
	}
	if (Input::IsKeyDown(VK_Y))
	{
		if (m_owner->GetComponent<Rigidbody2D>() == nullptr)
		{
			m_owner->AddComponent<Rigidbody2D>();
			if (auto rb = m_owner->GetComponent<Rigidbody2D>())
			{
				rb->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
				rb->gravityScale = 60.0f;
				rb->mass = 20.0f;
				rb->drag = 0.6f;
			}
		}
	}
	if (Input::IsKeyDown(VK_8))
	{
		m_owner->RemoveComponent<Collider>(m_owner->GetComponent<Collider>());
	}
	if (Input::IsKeyDown(VK_9))
	{
		if (m_owner->GetComponent<Collider>() == nullptr)
			m_owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 60));
	}


	if (Input::IsKeyDown(VK_C))
	{
		//m_owner->stateMachine->SetNextState(L"Idle");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		animInstance->SetBool("attack", true);
	}
	else
	{
		animInstance->SetBool("attack", false);
	}
	if (Input::IsKeyPressed(VK_SPACE))
	{
		if (jumpCount < maxJumpCount)
		{
			if(m_owner->GetComponent<Rigidbody2D>())
				m_owner->GetComponent<Rigidbody2D>()->AddForce(0, 720);
			jumpCount++;
		}
		//m_owner->GetComponent<Rigidbody2D>()->velocity.y = 150;
	}

	// �굥��ź �׽�Ʈ
	if (Input::IsKeyPressed(VK_G)) {
		if (auto prism = m_owner->GetComponent<Prism>())
			prism->SetActive(!prism->IsActive());
	}
}

void Player::OnCollisionEnter2D(Collision2D* collider)
{
    std::cout << "[Player] OnCollisionEnter2D ȣ���" << std::endl;
    OutputDebugStringW((L"[Player] OnCollisionEnter2D ȣ���" + std::to_wstring(EnterIndex++) + L"\n").c_str());
}

void Player::OnCollisionStay2D(Collision2D* collider)
{
    std::cout << "[Player] OnCollisionStay2D ȣ���" << std::endl;
    //OutputDebugStringW((L"[Player] OnCollisionStay2D ȣ���" + std::to_wstring(EnterIndex++) + L"\n").c_str());
}

void Player::OnCollisionExit2D(Collision2D* collider)
{
    std::cout << "[Player] OnCollisionExit2D ȣ���" << std::endl;
    OutputDebugStringW((L"[Player] OnCollisionExit2D ȣ���" + std::to_wstring(ExitIndex++) + L"\n").c_str());
}

void Player::OnTriggerEnter2D(Collider* collider)
{
    std::cout << "[Player] OnTriggerEnter2D ȣ���" << std::endl;
    OutputDebugStringW(L"[Player] OnTriggerEnter2D ȣ���\n");
}

void Player::OnTriggerStay2D(Collider* collider)
{
    std::cout << "[Player] OnTriggerStay2D ȣ���" << std::endl;
    OutputDebugStringW(L"[Player] OnTriggerStay2D ȣ���\n");
}

void Player::OnTriggerExit2D(Collider* collider)
{
    std::cout << "[Player] OnTriggerExit2D ȣ���" << std::endl;
    OutputDebugStringW(L"[Player] OnTriggerExit2D ȣ���\n");
}
