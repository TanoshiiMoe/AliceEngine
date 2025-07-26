#include "pch.h"
#include "UpdateTaskManager.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <System/ScriptSystem.h>

void UpdateTaskManager::Enque(WeakObjectPtr<ITickable> InTarget, Define::ETickingGroup InGroup, std::function<void(const float&)> TickFunc)
{
	m_TickLists[InGroup].emplace_back(InTarget, TickFunc);
}

void UpdateTaskManager::StartFrame()
{
	Context.DeltaSeconds = TimerManager::GetInstance().GetElapsedTime();
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_Awake);
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnStart);
}

void UpdateTaskManager::EndFrame()
{
	//ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnEnd);
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

void UpdateTaskManager::TickAll()
{
	for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
	{
		Context.TickGroup = static_cast<Define::ETickingGroup>(group);

		// 소멸된 객체면 TickList를 삭제하고 it는 그 자리에 그대로 있는다
		// 아니라면 it는 계속 탐색.
		// swap-and-pop을 고려하고 만든 코드
		for (auto it = m_TickLists[Context.TickGroup].begin(); it != m_TickLists[Context.TickGroup].end(); )
		{
			if (auto sp = it->Target.lock())
			{
				float delta = Context.DeltaSeconds;
				switch (Context.TickGroup)
				{
				case Define::ETickingGroup::TG_DuringPhysics:
					if (TimerManager::GetInstance().IfFixedUpdatable())
					{
						TimerManager::GetInstance().UpdateFixedTime(it->TickFunc);
						OutputDebugStringW((L"fixedTime : " + std::to_wstring(TimerManager::GetInstance().GetFixedTime()) + L"\n").c_str());
					}
					break;
				default:
					it->TickFunc(delta);
					break;
				}
				++it;
			}
			else
			{
				it = m_TickLists[Context.TickGroup].erase(it); // 소멸된 객체 제거
			}
		}
	}
}
