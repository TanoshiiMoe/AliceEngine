#include "pch.h"
#include "EnemyAnimatorInstance.h"
#include <Animation/AnimationController.h>

EnemyAnimatorInstance::EnemyAnimatorInstance()
{
}

EnemyAnimatorInstance::~EnemyAnimatorInstance()
{
}

void EnemyAnimatorInstance::Initialize()
{
	__super::Initialize();
}

void EnemyAnimatorInstance::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void EnemyAnimatorInstance::Release()
{
	__super::Release();
}

void EnemyAnimatorInstance::Render()
{
	__super::Render();
}

void EnemyAnimatorInstance::SetAnimatorController(AnimatorController* controller)
{
	__super::SetAnimatorController(controller);

	LoadSpriteSheet(L"ken_sprites.json");
	LoadAnimationClip(L"ken_kick_anim.json");
	LoadAnimationClip(L"ken_idle_anim.json");
	//SetLooping(true);
	Play();


	// 전체 State를 순회하면서 사용하는 AnimaitionClip의 길이를 가져와 State의 clipLength에 저장하기
	for (auto& state : m_Controller->states)
	{
		state.clipLength = animationClips[state.motionName]->duration;
	}
}

void EnemyAnimatorInstance::ChangeState(const std::string& stateName)
{
	__super::ChangeState(stateName);
	// 모션이름으로 현재 사용할 Animation Clip 연결. 
	std::cout << "ChangeState "
		<< m_Controller->states[m_CurrentStateIndex].motionName << std::endl;
	nextAnimationClip = m_Controller->states[m_CurrentStateIndex].motionName;
}

void EnemyAnimatorInstance::OnStart()
{
	__super::OnStart();
}

void EnemyAnimatorInstance::OnExit()
{

}

