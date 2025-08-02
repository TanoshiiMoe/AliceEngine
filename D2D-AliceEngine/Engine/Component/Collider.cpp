#include "pch.h"
#include "Collider.h"
#include <System/CollisionSystem.h>
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
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void Collider::Update(const float& deltaSeconds)
{
	if (collisionDelay > 0)
	{
		collisionDelay -= deltaSeconds;
		return;
	}
	else if (dirty)
	{
		dirty = false;
	}

	UpdateAABB();
}

void Collider::UpdateAABB()
{
	if (gameObject* owner = GetOwner())
	{
		FVector2 pos = owner->transform()->GetPosition();
		if (WeakObjectPtr<BoxComponent> comp = boxComponent)
		{
			aabb.minVector.x = pos.x - comp->GetBitmapSizeX() / 2;
			aabb.minVector.y = pos.y - comp->GetBitmapSizeY() / 2;
			aabb.maxVector.x = pos.x + comp->GetBitmapSizeX() / 2;
			aabb.maxVector.y = pos.y + comp->GetBitmapSizeY() / 2;
		}
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
		boxComponent->SetIgnoreOwnerScale(false);
		boxComponent->SetColor(FColor::Red);
		boxComponent->SetSize(_size);
		boxComponent->SetThickness(3.0f);
		boxComponent->m_layer = 999; // 디버그용 맨 앞에 그려지도록. 999
	}
	UpdateAABB();
}

void Collider::SetBoxColor(const FColor _color)
{
	if (boxComponent)
	{
		boxComponent->SetColor(_color);
	}
}
