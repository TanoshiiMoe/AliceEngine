#pragma once
#include <Object/UObject.h>
#include <Object/UObjectBase.h>

/*
* @brief : 이 클래스를 상속받아서 IdleState, AttackState등의 원하는 이름의 스테이트를 만듭니다.
*/
class Animator;
class AnimationClip;
class FSMState : public UObjectBase
{
public:
	FSMState();
	~FSMState();
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

	void SetAnimator(Animator* _animator);
	void SetAnimationClip(AnimationClip* _motion);

private:
	Animator* animator;
	AnimationClip* motion;
};

