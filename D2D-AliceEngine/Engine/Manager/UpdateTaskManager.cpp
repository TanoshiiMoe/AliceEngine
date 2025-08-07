#include "pch.h"
#include "UpdateTaskManager.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <System/ScriptSystem.h>

void UpdateTaskManager::Enque(WeakObjectPtr<ITickable> InTarget, Define::ETickingGroup InGroup, std::function<void(const float&)> TickFunc)
{
	m_TickLists[InGroup].emplace_back(InTarget, TickFunc);
}

void UpdateTaskManager::Dequeue(WeakObjectPtr<ITickable> InTarget)
{
	for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
	{
		auto& tickDeque = m_TickLists[static_cast<Define::ETickingGroup>(group)];

		for (auto it = tickDeque.begin(); it != tickDeque.end(); )
		{
			if (it->Target.lock() == InTarget.lock())
			{
				it = tickDeque.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
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

		// ���� TickList�� �����ϰ� ������ ����д�.
		auto& currentList = m_TickLists[Context.TickGroup];
		std::vector<UpdateWrapper> tempList;
		tempList.swap(currentList); // currentList�� ������� tempList�� ���� ������ ��

		for (auto it = tempList.begin(); it != tempList.end(); )
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
				it = tempList.erase(it); // �Ҹ�� ��ü ����
			}
		}

		// �̹� ������ �� �߰��� TickWrapper���� m_TickLists[Context.TickGroup]�� �̹� �׿� ����.
		// ��Ƴ��� ���� TickWrapper�� ���� �������� ���� �ٽ� �߰�.
		auto& nextTickList = m_TickLists[Context.TickGroup];
		nextTickList.insert(
			nextTickList.end(),
			std::make_move_iterator(tempList.begin()),
			std::make_move_iterator(tempList.end())
		);
	}
}
