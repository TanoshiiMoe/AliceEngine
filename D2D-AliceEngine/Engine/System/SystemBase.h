#pragma once
#include <Component/Component.h>
#include <Core/Singleton.h>

class SystemBase
{
public:
	SystemBase() {}
	virtual ~SystemBase() {}

public:
	void Regist(const WeakObjectPtr<Component>& _component)
	{
		if (!_component.expired())
		{
			m_Components.emplace_back(_component);
		}
	}

	void UnRegist(WeakObjectPtr<Component>&& _component)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).lock() && _component.lock())
			{
				if ((*it).lock() == _component.lock())
				{
					m_Components.erase(it);
					return;
				}
			}
		}
	}

	void UnRegistAll()
	{
		m_Components.clear();
	}

protected:
	std::vector<WeakObjectPtr<Component>> m_Components;
};

