#include "pch.h"
#include "FSMState.h"
#include <Component/Animator.h>
#include "FiniteStateMachine.h"

FSMState::FSMState()
{
	SetName(L"FSMState");
	animator = nullptr;
}

FSMState::~FSMState()
{
}

void FSMState::Update()
{
	if (!animator) return;
}

void FSMState::Enter()
{
	if (!animator) return;
	animator->SetAnimationClip(motion);
	animator->Play();
}

void FSMState::Exit()
{
	if (!animator) return;
	animator->Stop();
}

void FSMState::SetAnimator(Animator* _animator)
{
	animator = _animator;
}

void FSMState::SetStateMachine(FiniteStateMachine* _owner)
{
	owner = _owner;
}

FiniteStateMachine* FSMState::GetStateMachine()
{
	return owner;
}
