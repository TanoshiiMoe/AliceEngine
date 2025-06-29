#pragma once
#include "SystemBase.h"
#include <Component/ScriptComponent.h>
#include <unordered_map>
#include <queue>
#include <memory>
#include <functional>

#define REGISTER_SCRIPT_METHOD(Method) \
	ScriptSystem::GetInstance().Enque(weak_from_this(), Define::EScriptGroup::SG_##Method, \
		[weak = weak_from_this()]() { \
			if (auto sp = weak.lock()) \
			{ std::static_pointer_cast<std::remove_pointer<decltype(this)>::type>(sp)->Method(); } \
		})

struct ScriptWrapper
{
	std::weak_ptr<Component> Target;
	std::function<void()> TickFunc;

	ScriptWrapper(std::weak_ptr<Component> _Target, std::function<void()> _TickFunc)
		: Target(_Target), TickFunc(_TickFunc)
	{
	}
};

class ScriptSystem : public SystemBase, public Singleton<ScriptSystem>
{
public:
	Define::EScriptGroup currnetGroup;
	std::unordered_map<Define::EScriptGroup, std::queue<ScriptWrapper>> m_scriptQueues;

	void Enque(std::weak_ptr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc)
	{
		m_scriptQueues[InGroup].emplace(InTarget, TickFunc);
	}

    void ProcessScriptGroup(Define::EScriptGroup group)
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
};

