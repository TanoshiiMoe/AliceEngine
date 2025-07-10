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

void PhysicsSystem::Regist(const WeakObjectPtr<RigidBody2D>& _component)
{
	if (!_component.expired())
	{
		m_rigidBodies.emplace_back(_component);
	}
}

void PhysicsSystem::UnRegist(WeakObjectPtr<RigidBody2D>&& _component)
{
	m_rigidBodies.clear();
}

void PhysicsSystem::UnRegistAll()
{
	m_rigidBodies.clear();
}

void PhysicsSystem::Update(const float& deltaSeconds)
{

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
