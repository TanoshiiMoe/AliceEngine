#include "Car.h"
#include "System/ScriptSystem.h"
#include "Manager/UpdateTaskManager.h"
#include <Component/Animator.h>

void Car::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void Car::OnStart()
{
	owner->AddComponent<Animator>();
}

void Car::Update(const float& dt)
{
	if (isMoving)
		Move();
}

void Car::Move()
{
	
}

void Car::Stop()
{

}
