#include "pch.h"
#include "PlayerAnimatorInstance.h"
#include <Animation/AnimationController.h>
#include <Animation/TextureLoader.h>

PlayerAnimatorInstance::PlayerAnimatorInstance()
{
}

PlayerAnimatorInstance::~PlayerAnimatorInstance()
{
}

void PlayerAnimatorInstance::Initialize()
{
	__super::Initialize();
}

void PlayerAnimatorInstance::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void PlayerAnimatorInstance::Release()
{
	__super::Release();
}

void PlayerAnimatorInstance::Render()
{
	__super::Render();
}

void PlayerAnimatorInstance::RegisterParameter(const std::string& name, ParameterType type)
{
	m_ParameterTypes[name] = type;
}

void PlayerAnimatorInstance::SetInt(const std::string& name, int value)
{
	m_IntParameters[name] = value;
}

void PlayerAnimatorInstance::SetFloat(const std::string& name, float value)
{
	m_FloatParameters[name] = value;
}

void PlayerAnimatorInstance::SetBool(const std::string& name, bool value)
{
	m_BoolParameters[name] = value;
}

int PlayerAnimatorInstance::GetInt(const std::string& name) const
{
	auto it = m_IntParameters.find(name);
	if (it!= m_IntParameters.end()) 
	{
		return it->second;
	}
	return 0; // 기본값
}

bool PlayerAnimatorInstance::GetBool(const std::string& name) const
{
	auto it = m_BoolParameters.find(name);
	if (it != m_BoolParameters.end())
	{
		return it->second;
	}
	return false; // 기본값
}

float PlayerAnimatorInstance::GetFloat(const std::string& name) const
{
	auto it = m_FloatParameters.find(name);
	if (it != m_FloatParameters.end()) 
	{
		return it->second;
	}
	return 0.0f; // 기본값
}

bool PlayerAnimatorInstance::CheckCondition(const std::string& param, const std::string& mode, float threshold)
{
	auto it = m_ParameterTypes.find(param);
	if (it == m_ParameterTypes.end())
		return false;

	switch (it->second)
	{
	case ParameterType::Bool:
	{
		bool val = GetBool(param);
		if (mode == "If") return val;
		if (mode == "IfNot") return !val;
		break;
	}
	case ParameterType::Int:
	{
		int val = GetInt(param);
		if (mode == "Greater") return val > threshold;
		if (mode == "Less") return val < threshold;
		if (mode == "Equals") return val == static_cast<int>(threshold);
		if (mode == "NotEqual") return val != static_cast<int>(threshold);
		break;
	}
	case ParameterType::Float:
	{
		float val = GetFloat(param);
		if (mode == "Greater") return val > threshold;
		if (mode == "Less") return val < threshold;
		if (mode == "Equals") return val == threshold;
		if (mode == "NotEqual") return val != threshold;
		break;
	}
	default:
		return false;
	}
	return false;
}

bool PlayerAnimatorInstance::EvaluateConditions(const std::vector<Condition>& conditions)
{
	for (const auto& condition : conditions) 
	{
		if (!CheckCondition(condition.parameter, condition.mode, condition.threshold)) 
		{
			return false; // 하나라도 조건이 만족하지 않으면 false
		}
	}
	return true; // 모든 조건이 만족하면 true
}

void PlayerAnimatorInstance::SetAnimatorController(AnimatorController* controller)
{
	m_Controller = controller;
	// Parameter 초기화
	m_FloatParameters.clear();
	m_IntParameters.clear();
	m_BoolParameters.clear();
	for (const auto& param : controller->parameters)
	{
		if (param.type == "Int") {
			RegisterParameter(param.name, ParameterType::Int);
			SetInt(param.name, param.defaultInt);
		}
		else if (param.type == "Float") {
			RegisterParameter(param.name, ParameterType::Float);
			SetFloat(param.name, param.defaultFloat);
		}
		else if (param.type == "Bool") {
			RegisterParameter(param.name, ParameterType::Bool);
			SetBool(param.name, param.defaultBool);
		}
	}

	// 상태별로 인터페이스 주소를 보관할 공간을 확보한다.
	m_StateNotifiers.clear();
	for (auto& state : m_Controller->states) 
	{
		m_StateNotifiers.push_back(nullptr);
	}

	LoadSpriteSheet("Zero\\Zero_sprites.json");
	LoadAnimationClip("Zero\\Zero_idle_anim.json");
	LoadAnimationClip("Zero\\Zero_attack_anim.json");
	SetLooping(true);

	// ToDo: 전체 State를 순회하면서 사용하는 AnimaitionClip의 길이를 가져와 
	// State의 clipLength에 저장해야 한다.
	for (auto& state : m_Controller->states)
	{
		state.clipLength = animationClips[state.motionName]->duration;
	}
}

void PlayerAnimatorInstance::ChangeState(const std::string& stateName)
{
	assert(m_Controller != nullptr);

	State* pNext = m_Controller->GetState(stateName);
	if (pNext == nullptr || pNext->index == m_CurrentStateIndex)
		return;

	m_PrevStateIndex = m_CurrentStateIndex;
	m_CurrentStateIndex = pNext->index;
	m_elapsedTime = 0.0f; // ← 전이 후 시간 초기화

	if (m_PrevStateIndex != -1 && m_StateNotifiers[m_PrevStateIndex] != nullptr)
	{

		m_StateNotifiers[m_PrevStateIndex]->OnExitState(
			m_Controller->states[m_PrevStateIndex].name);
	}

	if (m_StateNotifiers[m_CurrentStateIndex] != nullptr) {
		m_StateNotifiers[m_CurrentStateIndex]->OnEnterState(
			m_Controller->states[m_CurrentStateIndex].name);
	}

	// ToDo:: 모션이름으로 현재 사용할 Animation Clip 참조 연결해야합니다. 
	std::cout << "ChangeState "
		<< m_Controller->states[m_CurrentStateIndex].motionName << std::endl;
	nextAnimationClip = m_Controller->states[m_CurrentStateIndex].motionName;
}

void PlayerAnimatorInstance::Update(float deltaTime)
{
	if (m_CurrentStateIndex < 0)
		return;

	m_elapsedTime += deltaTime;
	// 재생 비율 계산 0.0~1.0
	float clipLength = m_Controller->states[m_CurrentStateIndex].clipLength;
	float elapsedRate = m_elapsedTime / clipLength;

	if (m_elapsedTime > clipLength && m_Controller->states[m_CurrentStateIndex].loop)
	{
		m_elapsedTime = fmod(m_elapsedTime, clipLength); // 나머지 시간
	}

	// Todo: SpriteRender에 현재시간에 맞는 Sprite정보를 설정해야합니다.	


	std::cout << "State "
		<< m_Controller->states[m_CurrentStateIndex].name
		<< ", Elapsed Time: " << m_elapsedTime << std::endl;

	if (m_StateNotifiers[m_CurrentStateIndex] != nullptr) {
		m_StateNotifiers[m_CurrentStateIndex]->OnUpdateState(
			m_Controller->states[m_CurrentStateIndex].name);
	}

	const auto& currentStateDef = m_Controller->states.at(m_CurrentStateIndex);

	for (const auto& trans : currentStateDef.transitions) {
		// 해당 트랜지션이 exitTime을 사용하는 경우 재생율 비교하여 클때만 검사   
		if (trans.hasExitTime && elapsedRate < trans.exitTime)
			continue;

		// false이면 매번 프레임 평가
		if (EvaluateConditions(trans.conditions)) {
			std::cout << "Transition "
				<< m_Controller->states[m_CurrentStateIndex].name << " -> "
				<< trans.toState << " " << m_elapsedTime << std::endl;
			ChangeState(trans.toState);
			return;
		}
	}

	// 상태와 상관없이 Any State 전이 검사
	for (const auto& trans : m_Controller->anyStateTransitions) {
		if (EvaluateConditions(trans.conditions)) {
			std::cout << "AnyStateTransition "
				<< m_Controller->states[m_CurrentStateIndex].name << " -> "
				<< trans.toState << " " << m_elapsedTime << std::endl;
			ChangeState(trans.toState);
			return;
		}
	}
}

void PlayerAnimatorInstance::OnStart()
{
	if (m_Controller)
		ChangeState(m_Controller->defaultState); // 기본 상태로 전이
}

void PlayerAnimatorInstance::OnExit()
{

}

