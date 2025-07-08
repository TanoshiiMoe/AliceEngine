#pragma once
#include <Core/Singleton.h>
#include <Experimental/Physics/AABB.h>
class Collider;
class PhysicsSystem : public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem() {}
	virtual ~PhysicsSystem() {}

public:
	void Regist(const WeakObjectPtr<Collider>& _component)
	{
		if (!_component.expired())
		{
			m_AABBs.emplace_back(_component);
		}
	}

	void UnRegist(WeakObjectPtr<Collider>&& _component)
	{
		for (auto it = m_AABBs.begin(); it != m_AABBs.end(); ++it)
		{
			if ((*it).lock() && _component.lock())
			{
				if ((*it).lock() == _component.lock())
				{
					m_AABBs.erase(it);
					return;
				}
			}
		}
	}

	void UnRegistAll()
	{
		m_AABBs.clear();
	}

protected:
	std::vector<WeakObjectPtr<Collider>> m_AABBs;
};

