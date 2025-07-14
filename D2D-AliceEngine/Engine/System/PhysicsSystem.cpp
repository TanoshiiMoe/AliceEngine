#include "pch.h"
#include "PhysicsSystem.h"
#include <Manager/UpdateTaskManager.h>
#include <Core/Tickable.h>
#include <Core/ObjectHandler.h>
#include <Experimental/Collision/CollisionDetector.h>

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Regist(const WeakObjectPtr<Rigidbody2D>& _component)
{
	if (!_component.expired())
	{
		m_rigidBodies.emplace_back(_component);
	}
}

void PhysicsSystem::UnRegist(WeakObjectPtr<Rigidbody2D>&& _component)
{
	m_rigidBodies.clear();
}

void PhysicsSystem::UnRegistAll()
{
	m_rigidBodies.clear();
}

void PhysicsSystem::Update(const float& deltaSeconds)
{
	for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end(); )
	{
		if (it->expired())
		{
			it = m_rigidBodies.erase(it);
		}
		else
		{
			Rigidbody2D* rb = it->Get();
			// ºÎµúÃÆ´Ù¸é
			if (collidedBodies.find(rb) != collidedBodies.end())
			{
				rb->isGrounded = true;
			}
			// ºÎµúÄ¡Áö ¾Ê¾Ò´Ù¸é
			else
			{
				rb->Update(deltaSeconds);
			}
			it++;
		}
	}
}

void PhysicsSystem::Initialize()
{
	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_DuringPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void PhysicsSystem::Update()
{
}

void PhysicsSystem::Release()
{
}
