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
			m_owner->transform()->AddPosition(0, -speed);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, speed);
		else
			m_owner->transform()->AddPosition(0, speed);
	}
	// ���� ī��Ʈ ����: ���� ������ jumpCount = 0
	auto rb = m_owner->GetComponent<Rigidbody2D>();
	bool isGround = (rb->m_eRigidBodyState == Define::ERigidBodyState::Ground ||
					 rb->m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody);
	if (isGround && prevGroundState == 0) {
			jumpCount = 0; // ���� ó�� ����� ���� ����
		}
	prevGroundState = isGround ? 1 : 0;
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
		rb->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
		rb->gravityScale = 60;
		rb->mass = 20;
		rb->drag = 0.6;
		//rb->angularDrag = 0;
	}

	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });
}

void Player::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Player::OnDestroy()
{
}

void Player::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�

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
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, 720);
			jumpCount++;
		}
		//m_owner->GetComponent<Rigidbody2D>()->velocity.y = 150;
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
