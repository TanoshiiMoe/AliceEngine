#pragma once
#include <FSM/FSMState.h>

class AttackState : public FSMState
{
public:
	virtual void Update() override;
	virtual void Enter() override;
	virtual void Exit() override;
};

