#include "pch.h"
#include "Collider.h"
#include <System/PhysicsSystem.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Component/TransformComponent.h>
#include <Component/BoxComponent.h>
#include <Object/gameObject.h>

Collider::Collider()
{

}

Collider::~Collider()
{
	CollisionSystem::GetInstance().UnRegist(WeakFromThis<Collider>());
}

void Collider::Initialize()
{
	CollisionSystem::GetInstance().Regist(WeakFromThis<Collider>());

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PrePhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void Collider::Update(const float& deltaSeconds)
{
	if (gameObject* owner = GetOwner())
	{
		FVector2 pos = owner->transform()->GetPosition();
		if (WeakObjectPtr<BoxComponent> comp = boxComponent)
		{
			aabb.minVector.x = pos.x - comp->GetSizeX() / 2;
			aabb.minVector.y = pos.y - comp->GetSizeY() / 2;
			aabb.maxVector.x = pos.x + comp->GetSizeX() / 2;
			aabb.maxVector.y = pos.y + comp->GetSizeY() / 2;
		}
	}
	if (dirty)
	{
		dirty = false;
	}
}

void Collider::Release()
{
}

void Collider::SetBoxSize(const FVector2& _size)
{
	WeakObjectPtr<BoxComponent> comp = boxComponent;
	if (comp.expired())
	{
		boxComponent = GetOwner()->AddComponent<BoxComponent>();
		boxComponent->SetBoxType(Define::EBoxType::ColliderDebugBox);
	}
	boxComponent->SetColor(FColor::Red);
	boxComponent->SetSize(_size);
	boxComponent->SetThickness(3.0f);

	if (gameObject* owner = GetOwner())
	{
		FVector2 pos = owner->transform()->GetPosition();
		aabb.minVector.x = pos.x - _size.x / 2;
		aabb.minVector.y = pos.y - _size.y / 2;
		aabb.maxVector.x = pos.x + _size.x / 2;
		aabb.maxVector.y = pos.y + _size.y / 2;
	}
}