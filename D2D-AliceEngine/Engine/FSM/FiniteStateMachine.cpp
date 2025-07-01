#include "pch.h"
#include "FiniteStateMachine.h"
#include <FSM/FSMState.h>

FiniteStateMachine::FiniteStateMachine()
{
	SetName(L"FiniteStateMachine");
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::Update()
{
	if (m_pCurState == nullptr && m_pNextState == nullptr) return;
	if (m_pCurState == nullptr && m_pNextState != nullptr)
	{
		m_pNextState->Enter();
		m_pCurState = m_pNextState;
		return;
	}
	if (m_pCurState != m_pNextState)
	{
		m_pCurState->Exit();
		m_pNextState->Enter();
		m_pCurState = m_pNextState;
		return;
	}
	m_pCurState->Update();
}

void FiniteStateMachine::SetNextState(const std::wstring& _stateName)
{
	if (m_StateContiner.find(_stateName) == m_StateContiner.end())
		return;
	m_pNextState = m_StateContiner[_stateName];
}

void FiniteStateMachine::CreateState(const std::wstring& _stateName, FSMState* _state)
{
	m_StateContiner[_stateName] = _state;
}
