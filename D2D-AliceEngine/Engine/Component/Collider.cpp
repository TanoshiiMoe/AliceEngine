#include "pch.h"
#include "Collider.h"
#include <System/PhysicsSystem.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>

Collider::Collider()
{
	
}

Collider::~Collider()
{
	PhysicsSystem::GetInstance().UnRegist(WeakFromThis<Collider>());
}

void Collider::Initialize()
{
	PhysicsSystem::GetInstance().Regist(WeakFromThis<Collider>());

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

void Collider::Update(const float& deltaSeconds)
{
	if (bShowColliderBox == false) return;

	DrawDebugAABBBox();
}

void Collider::Release()
{
}

void Collider::SetAABBBoxSize(const FVector2& minVector, const FVector2& maxVector)
{
	aabb.minVector = minVector;
	aabb.maxVector = maxVector;
}

// Update때 박스 그려서 보여주기
void Collider::DrawDebugAABBBox()
{
	if (gameObject* owner = GetOwner())
	{
		FVector2 pos = owner->transform()->GetPosition();
		FVector2 size = aabb.GetSize() / 2;
		const float startPosX = pos.x - size.x;
		const float startPosY = pos.y - size.y;
		const float ensPosX = pos.x + size.x;
		const float ensPosY = pos.y + size.y;
		D2DRenderManager::GetInstance().DrawDebugBox(startPosX, startPosY, ensPosX, ensPosY);
	}
}

bool Collider::IsOverlap(Collider* other)
{
	if (WeakObjectPtr<Collider> otherWeak = other)
	{

	}
}
