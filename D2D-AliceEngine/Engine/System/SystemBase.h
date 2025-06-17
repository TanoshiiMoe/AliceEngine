#pragma once
#include <Component/Component.h>
#include <Core/Singleton.h>

class SystemBase
{
public:
	SystemBase() {}
	~SystemBase() {}

public:
	void Regist(const std::weak_ptr<Component>& _component)
	{
		m_Components.emplace_back(_component);
	}

	void UnRegist(std::weak_ptr<Component>&& _component)
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
	std::vector<std::weak_ptr<Component>> m_Components;
};

