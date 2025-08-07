#include "Car.h"
#include "System/ScriptSystem.h"
#include "Manager/UpdateTaskManager.h"
#include <Component/Animator.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>

void Car::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void Car::OnStart()
{
	//애니메이터 있을시
	//owner->AddComponent<Animator>();
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();
}

void Car::Update(const float& dt)
{
	if (isMoving)
		Move(dt);
}

void Car::Move(const float& dt)
{
	owner->transform()->AddPosition(moveSpeed * dt, 0.0f);
}

void Car::MoveUp()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveUp();
}

void Car::MoveDown()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveDown();
}

void Car::Die()
{
	dieActions.BroadCast();
}

void Car::AddDieAction(ObjectHandle _handle, std::function<void()> _func)
{
	dieActions.Add(_handle, _func);
}

void Car::RemoveDieAction(ObjectHandle _handle)
{
	dieActions.Remove(_handle);
}

void Car::ClearDieAction()
{
	dieActions.Clear();
}
