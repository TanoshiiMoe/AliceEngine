#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>

void Physics::FCollisionDetector::BruteForceOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if(objects[i].expired()) continue;
		if (objects[i]->dirty) continue;
		for (size_t j = i + 1; j < objects.size(); ++j)
		{
			if (objects[j].expired()) continue;

			if (IsOverlapped(objects[i],objects[j]))
			{
                PushOverlappedArea(objects[i].Get(), objects[j].Get());
				std::wcout << L"[충돌] " << objects[i]->GetName()<< " " << objects[j]->GetName() << " 겹침\n";
			}
		}
	}
}

void Physics::FCollisionDetector::SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
    // Sweep and Prune을 위한 정렬
    std::sort(objects.begin(), objects.end(), &FCollisionDetector::CompareColliderMinX);
    
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->dirty) continue;
        auto& src = objects[i];

        for (size_t j = i + 1; j < objects.size(); ++j)
        {
            if (objects[j]->dirty) continue;
            auto& tar = objects[j];
            // Prune: 더 이상 겹칠 수 없으면 break
            if (tar->aabb.minVector.x > src->aabb.maxVector.x)
                break;

            // 실제 충돌 검사
            if (IsOverlapped(src, tar))
            {
                PushOverlappedArea(src.Get(), tar.Get());
                std::wcout << L"[충돌] " << src->GetName() << L" " << tar->GetName() << L" 겹침\n";
            }
        }
    }
}

bool Physics::FCollisionDetector::CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
    return a->aabb.minVector.x < b->aabb.minVector.x;
}

bool Physics::FCollisionDetector::IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
	return !(a.Get()->aabb.minVector.x > b.Get()->aabb.maxVector.x ||
		a.Get()->aabb.minVector.y > b.Get()->aabb.maxVector.y ||
		a.Get()->aabb.maxVector.x < b.Get()->aabb.minVector.x ||
		a.Get()->aabb.maxVector.y < b.Get()->aabb.minVector.y);
}

void Physics::FCollisionDetector::PushOverlappedArea(Collider* a, Collider* b)
{
    // aabb 정보 가져오기
    const auto& aabb_a = a->aabb;
    const auto& aabb_b = b->aabb;

    // 겹친 거리 계산
    float overlap_x = min(aabb_a.maxVector.x, aabb_b.maxVector.x) - max(aabb_a.minVector.x, aabb_b.minVector.x);
    float overlap_y = min(aabb_a.maxVector.y, aabb_b.maxVector.y) - max(aabb_a.minVector.y, aabb_b.minVector.y);

    if (overlap_x < overlap_y)
    {
        float push = overlap_x / 2.0f;
        if (aabb_a.minVector.x < aabb_b.minVector.x)
        {
            a->GetOwner()->transform()->AddPosition(-push, 0);
            b->GetOwner()->transform()->AddPosition(push, 0);
        }
        else
        {
            a->GetOwner()->transform()->AddPosition(push, 0);
            b->GetOwner()->transform()->AddPosition(-push, 0);
        }
    }
    else
    {
        float push = overlap_y / 2.0f;
        if (aabb_a.minVector.y < aabb_b.minVector.y)
        {
            a->GetOwner()->transform()->AddPosition(0, -push);
            b->GetOwner()->transform()->AddPosition(0, push);
        }
        else
        {
            a->GetOwner()->transform()->AddPosition(0, push);
            b->GetOwner()->transform()->AddPosition(0, -push);
        }
    }
}
