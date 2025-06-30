#include "pch.h"
#include "UpdateTaskManager.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <System/ScriptSystem.h>

void UpdateTaskManager::StartFrame()
{
	Context.DeltaSeconds = TimerManager::GetInstance().GetElapsedTime();
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_Awake);
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnStart);
}

void UpdateTaskManager::EndFrame()
{
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnEnd);
}

void UpdateTaskManager::SetWorld()
{
	Context.World = SceneManager::GetInstance().GetWorld();
}

void UpdateTaskManager::ClearWorld()
{
	Context.World = nullptr;
	m_TickLists.clear();
}
