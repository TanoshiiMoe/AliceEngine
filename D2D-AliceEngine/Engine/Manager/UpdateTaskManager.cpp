#include "pch.h"
#include "UpdateTaskManager.h"
#include <Manager/SceneManager.h>

void UpdateTaskManager::SetWorld()
{
	Context.World = SceneManager::GetInstance().GetWorld();
}

void UpdateTaskManager::ClearWorld()
{
	Context.World = nullptr;
	m_TickLists.clear();
}
