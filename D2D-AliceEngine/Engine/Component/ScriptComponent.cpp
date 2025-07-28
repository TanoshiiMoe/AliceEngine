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
	ScriptSystem::GetInstance().UnRegist(WeakFromThis<Component>());
}

void ScriptComponent::Initialize()
{
	ScriptSystem::GetInstance().Regist(WeakFromThis<Component>());

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_DuringPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			if (auto script = dynamic_cast<ScriptComponent*>(sp))
			{
				script->FixedUpdate(dt);
			}
		}
	}
	);

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostUpdateWork,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			if (auto script = dynamic_cast<ScriptComponent*>(sp))
			{
				script->LateUpdate(dt);
			}
		}
	}
	);
}

void ScriptComponent::FixedUpdate(const float& deltaSeconds)
{
	OutputDebugStringW((L"FixedUpdate(deltaSeconds) : " + std::to_wstring(deltaSeconds) + L"\n").c_str());
}

void ScriptComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	//OutputDebugStringW((L"Update(deltaSeconds) : " + std::to_wstring(deltaSeconds) + L"\n").c_str());
}

void ScriptComponent::LateUpdate(const float& deltaSeconds)
{

}

void ScriptComponent::Release()
{
	
}

void ScriptComponent::Awake()
{
}

void ScriptComponent::OnStart()
{
}

void ScriptComponent::OnEnd()
{
}

void ScriptComponent::OnDestroy()
{
}

Scene* ScriptComponent::GetWorld()
{
	return SceneManager::GetInstance().GetWorld();
}
