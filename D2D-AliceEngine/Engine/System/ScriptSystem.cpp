#include "pch.h"
#include "ScriptSystem.h"

void ScriptSystem::Enque(WeakObjectPtr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc)
{
	m_scriptQueues[InGroup].emplace(InTarget, TickFunc);
}

void ScriptSystem::Enque(WeakObjectPtr<Component> InTarget, Define::ESCriptUpdateGroup InGroup, std::function<void()> TickFunc)
{
	m_scriptTickingQueues[InGroup].emplace_back(InTarget, TickFunc);
}

void ScriptSystem::ProcessScriptGroup(Define::EScriptGroup group)
{
	currnetGroup = group;
	while (m_scriptQueues[group].empty() == false)
	{
		if (!m_scriptQueues[group].front().Target.expired())
		{
			m_scriptQueues[group].front().TickFunc();
		}
		m_scriptQueues[group].pop();
	}
}

void ScriptSystem::ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup group)
{
	currnetTickingGroup = group;

	for (auto it = m_scriptTickingQueues[group].begin(); it != m_scriptTickingQueues[group].end(); )
	{
		if (it->Target.expired() == false)
		{
			it->TickFunc();
			++it;
		}
		else
		{
			it = m_scriptTickingQueues[group].erase(it); // 소멸된 객체 제거
		}
	}
}
