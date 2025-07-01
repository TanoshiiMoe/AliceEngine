#include "AttackState.h"
#include <Component/Animator.h>
#include <FSM/FiniteStateMachine.h>

void AttackState::Update()
{
	__super::Update();
	if (animator->IsEnd())
	{
		GetStateMachine()->SetNextState(L"Idle");
	}
}

void AttackState::Enter()
{
	__super::Enter();
}

void AttackState::Exit()
{
	__super::Exit();
}
