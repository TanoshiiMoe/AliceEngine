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
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Player::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	float speed = 55.0f * deltaSeconds;
	if (!(Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_DOWN) || Input::IsKeyDown(VK_UP)))
	{
		animInstance->SetFloat("speed", 0);
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_owner->transform()->AddPosition(speed, 0);
		animInstance->SetFlip(true);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_owner->transform()->AddPosition(-speed, 0);
		animInstance->SetFlip(false);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		m_owner->transform()->AddPosition(0, -55.0f * deltaSeconds);
	}
	if (Input::IsKeyDown(VK_UP))
	{
		m_owner->transform()->AddPosition(0, 55.0f * deltaSeconds);
	}
}

void Player::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Player::Awake()
{
}

void Player::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
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

	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });
}

void Player::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void Player::OnDestroy()
{
}

void Player::Input()
{
	// 여기에 Input에 대한 로직 작성

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
}
