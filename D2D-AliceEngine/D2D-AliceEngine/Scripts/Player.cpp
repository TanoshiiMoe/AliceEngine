#include "Player.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <System/ScriptSystem.h>

#include <Helpers/TextureLoader.h>
#include <Component/Animator.h>
#include <FSM/FiniteStateMachine.h>
#include <fsm/FSMState.h>
#include "../FSMState/IdleState.h"
#include "../FSMState/AttackState.h"

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

	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_owner->transform()->AddPosition(55.0f * deltaSeconds, 0);
		m_animator->SetFlip(true);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_owner->transform()->AddPosition(-55.0f * deltaSeconds, 0);
		m_animator->SetFlip(false);
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

	Texture = std::make_shared<SpriteSheet>();
	idle = std::make_shared<AnimationClip>();
	attack = std::make_shared<AnimationClip>();

	/*Animator::LoadSpriteSheet("ken_sprites.json", Texture);
	Animator::LoadAnimationClip("ken_kick_anim.json", kick, Texture);
	Animator::LoadAnimationClip("ken_idle_anim.json", idle, Texture);*/

	Animator::LoadSpriteSheet("Zero\\Zero_sprites.json", Texture);
	Animator::LoadAnimationClip("Zero\\Zero_attack_anim.json", attack, Texture);
	Animator::LoadAnimationClip("Zero\\Zero_idle_anim.json", idle, Texture);

	m_animator = m_owner->AddComponent<Animator>();
	m_animator->LoadSpriteRenderer(Texture);
	m_animator->SetLooping(true);

	idleState = new IdleState();
	attackState = new AttackState();
	idleState->SetAnimator(m_animator);
	attackState->SetAnimator(m_animator);
	idleState->SetAnimationClip(idle.get());
	attackState->SetAnimationClip(attack.get());

	m_owner->stateMachine->CreateState(L"Idle", idleState);
	m_owner->stateMachine->CreateState(L"Attack", attackState);
	m_owner->stateMachine->SetNextState(L"Idle");

	m_owner->AddComponent<InputComponent>()->SetAction(m_owner, [this]() { Input(); });
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
		m_owner->stateMachine->SetNextState(L"Idle");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		m_owner->stateMachine->SetNextState(L"Attack");
	}
}
