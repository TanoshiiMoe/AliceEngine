#include "pch.h"
#include "InputComponent.h"
#include <Manager/UpdateTaskManager.h>

InputComponent::InputComponent()
{
}

InputComponent::~InputComponent()
{
	InputSystem::GetInstance().UnRegist(this->weak_from_this());
	actions.clear();
}

void InputComponent::Initialize()
{
	InputSystem::GetInstance().Regist(this->weak_from_this());

	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PrePhysics,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void InputComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	for (auto action : actions)
	{
		action();
	}
}

void InputComponent::Release()
{
	actions.clear();
}
