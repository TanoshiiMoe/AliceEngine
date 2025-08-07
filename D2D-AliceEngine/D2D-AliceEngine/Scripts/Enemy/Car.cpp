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
	owner->AddComponent<Animator>();
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

void Car::Stop()
{

}
