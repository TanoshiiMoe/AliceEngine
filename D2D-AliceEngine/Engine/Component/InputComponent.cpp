#include "pch.h"
#include "InputComponent.h"
#include <Manager/UpdateTaskManager.h>

InputComponent::InputComponent()
{
	InputSystem::GetInstance().Regist(WeakFromThis<Component>());
}

InputComponent::~InputComponent()
{
	InputSystem::GetInstance().UnRegist(WeakFromThis<Component>());
	slots.clear();
}

void InputComponent::Initialize()
{
	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PrePhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
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
	for(auto it = slots.begin(); it != slots.end();)
	{
		if (ObjectHandler::GetInstance().IsValid(it->handle))
		{
			it->func();
			it++;
		}
		else
		{
			it = slots.erase(it);
		}
	}
}

void InputComponent::Release()
{
	slots.clear();
}
