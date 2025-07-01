#pragma once
#include <unordered_map>
#include <Object/UObjectBase.h>

class FSMState;
class FiniteStateMachine : public UObjectBase
{
public:
	FiniteStateMachine();
	~FiniteStateMachine();

	FSMState* m_pCurState;
	FSMState* m_pNextState;
	std::unordered_map<std::wstring, FSMState*> m_StateContiner;

	void Update();
	void SetNextState(const std::wstring& _stateName);
	void CreateState(const std::wstring& _stateName, FSMState* _state);
};