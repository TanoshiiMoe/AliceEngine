#include "pch.h"
#include "CollisionSystem.h"
#include <Manager/UpdateTaskManager.h>
#include <Core/Tickable.h>
#include <Core/ObjectHandler.h>
#include <Experimental/Collision/CollisionDetector.h>
#include <System/PhysicsSystem.h>
#include <Component/Rigidbody2D.h>

CollisionSystem::CollisionSystem()
{
	
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Regist(const WeakObjectPtr<Collider>& _component)
{
	if (!_component.expired())
	{
		m_AABBs.emplace_back(_component);
	}
}

void CollisionSystem::UnRegist(WeakObjectPtr<Collider>&& _component)
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

void CollisionSystem::UnRegistAll()
{
	m_AABBs.clear();
}

void CollisionSystem::Update(const float& deltaSeconds)
{
	PhysicsSystem::GetInstance().collidedBodies = Physics::FCollisionDetector::SweepAndPruneOverlapCheck(m_AABBs);
	//Physics::FCollisionDetector::BruteForceOverlapCheck(m_AABBs);
}

void CollisionSystem::Initialize()
{
	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_StartPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void CollisionSystem::Update()
{
}

void CollisionSystem::Release()
{
}
