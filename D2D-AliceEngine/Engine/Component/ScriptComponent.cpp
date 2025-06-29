#include "pch.h"
#include "ScriptComponent.h"
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Scene/Scene.h>

ScriptComponent::ScriptComponent()
{

}

ScriptComponent::~ScriptComponent()
{
	ScriptSystem::GetInstance().UnRegist(this->weak_from_this());
}

void ScriptComponent::Initialize()
{
	ScriptSystem::GetInstance().Regist(this->weak_from_this());
	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);

	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_DuringPhysics,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			if (auto script = std::dynamic_pointer_cast<ScriptComponent>(sp))
			{
				script->FixedUpdate(dt);
			}
		}
	}
	);

	UpdateTaskManager::GetInstance().Enque(
		weak_from_this(),
		Define::ETickingGroup::TG_PostUpdateWork,
		[weak = weak_from_this()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			if (auto script = std::dynamic_pointer_cast<ScriptComponent>(sp))
			{
				script->LateUpdate(dt);
			}
		}
	}
	);
}

void ScriptComponent::FixedUpdate(const float& deltaSeconds)
{

}

void ScriptComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void ScriptComponent::LateUpdate(const float& deltaSeconds)
{

}

void ScriptComponent::Release()
{
	
}

void ScriptComponent::OnStart()
{
}

void ScriptComponent::OnEnd()
{
}

Scene* ScriptComponent::GetWorld()
{
	return SceneManager::GetInstance().GetWorld();
}
