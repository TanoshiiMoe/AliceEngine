#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/Rigidbody2D.h>

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

std::unordered_set<Rigidbody2D*> Physics::FCollisionDetector::SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
    // Sweep and Prune을 위한 정렬
    std::sort(objects.begin(), objects.end(), &FCollisionDetector::CompareColliderMinX);
    
    std::unordered_set<Rigidbody2D*> overlappedRigdBodies;
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
				if (Rigidbody2D* rbA = src->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rbA);
				if (Rigidbody2D* rbB = tar->GetOwner()->GetComponent<Rigidbody2D>()) overlappedRigdBodies.insert(rbB);

                /*auto typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
                auto typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;*/

                //// Dynamic-Static(또는 Kinematic) 충돌만 set에 추가
                //if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic))
                //    ovelappedRigdBodies.insert(rbA);
                //if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic)
                //    ovelappedRigdBodies.insert(rbB);
                //// Dynamic-Dynamic끼리는 넣지 않음!
            }
        }
    }
    return overlappedRigdBodies;
}

bool Physics::FCollisionDetector::CompareColliderMinX(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
    if (a->aabb.minVector.x < b->aabb.minVector.x)
    {
        return true;
    }
    else if (a->aabb.minVector.x > b->aabb.minVector.x)
    {
        return false;
    }
    else
    {
        return a->aabb.minVector.y < b->aabb.minVector.y;
    }
}

bool Physics::FCollisionDetector::IsOverlapped(const WeakObjectPtr<Collider>& a, const WeakObjectPtr<Collider>& b)
{
	return !(a.Get()->aabb.minVector.x > b.Get()->aabb.maxVector.x ||
		a.Get()->aabb.minVector.y > b.Get()->aabb.maxVector.y ||
		a.Get()->aabb.maxVector.x < b.Get()->aabb.minVector.x ||
		a.Get()->aabb.maxVector.y < b.Get()->aabb.minVector.y);
}

// 충돌을 감지하고 AABB 오버랩 된 부분을 밀어내는 로직입니다.
// 각 상태를 나누어 관리합니다. 
// 1. 땅 위
// 2. 땅 위에 있는 오브젝트의 위
// 3. 공기 중
//
void Physics::FCollisionDetector::PushOverlappedArea(Collider* a, Collider* b)
{
	FAABB& aabb_a = a->aabb;
	FAABB& aabb_b = b->aabb;

	Rigidbody2D* rbA = a->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = b->GetOwner()->GetComponent<Rigidbody2D>();

	auto massA = rbA ? rbA->mass : FLT_MAX;
	auto massB = rbB ? rbB->mass : FLT_MAX;
	auto typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
	auto typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;

	// 겹친 거리 계산
	float overlap_x = min(aabb_a.maxVector.x, aabb_b.maxVector.x) - max(aabb_a.minVector.x, aabb_b.minVector.x);
    float overlap_y = min(aabb_a.maxVector.y, aabb_b.maxVector.y) - max(aabb_a.minVector.y, aabb_b.minVector.y);

	// 밀림 방향 결정
	float pushX = 0, pushY = 0;
	if (overlap_x < overlap_y) 
    {
		pushX = (aabb_a.minVector.x < aabb_b.minVector.x) ? -overlap_x : overlap_x;
	}
    else 
    {
		pushY = (aabb_a.minVector.y < aabb_b.minVector.y) ? -overlap_y : overlap_y;
	}

    float weight = 1.1f;
    pushX *= weight;
    pushY *= weight;

	float totalMass = massA + massB;
	float ratioA = massA / totalMass;
	float ratioB = massB / totalMass;

	if (typeA == Define::ERigidBodyType::Dynamic && typeB == Define::ERigidBodyType::Dynamic)
    {
        // 중심점 계산
        float centerA_y = (aabb_a.minVector.y + aabb_a.maxVector.y) * 0.5f;
        float centerB_y = (aabb_b.minVector.y + aabb_b.maxVector.y) * 0.5f;

        Rigidbody2D* upper = nullptr;
        Rigidbody2D* lower = nullptr;

        // y축 기준 위/아래 판별
        if (centerA_y > centerB_y) {
            upper = rbA;
            lower = rbB;
        }
        else {
            upper = rbB;
            lower = rbA;
        }

        // upper(위에 있는 오브젝트) 상태 전이
        // y축 기준이니까 위에 있는 오브젝트, 아래에 있는 오브젝트를 나누자.
        // 무조건 위에 있는 오브젝트를 기준으로 상태를 나누자.
        switch (upper->m_eRigidBodyState)
        {
        case Define::ERigidBodyState::Space:
            switch (lower->m_eRigidBodyState)
            {
            case Define::ERigidBodyState::Space:
                // 둘 다 공중: 변화 없음
                break;
            case Define::ERigidBodyState::Ground:
                upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
                break;
            case Define::ERigidBodyState::OnRigidBody:
                upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
                break;
            }
            break;
        case Define::ERigidBodyState::Ground:
            // 위에 있는 애가 Ground일 수는 거의 없음(특수상황)
            break;
        case Define::ERigidBodyState::OnRigidBody:
            switch (lower->m_eRigidBodyState)
            {
            case Define::ERigidBodyState::Space:
                upper->m_eRigidBodyState = Define::ERigidBodyState::Space;
                break;
            case Define::ERigidBodyState::Ground:
                // 아래가 Ground면 위는 OnRigidBody 유지
                break;
            case Define::ERigidBodyState::OnRigidBody:
                break;
            }
            break;
        }

        // 둘 다 밀림 (질량 비율)
        a->GetOwner()->transform()->AddPosition(pushX * ratioA, pushY * ratioA);
        b->GetOwner()->transform()->AddPosition(-pushX * ratioB, -pushY * ratioB);
	}
	else if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic)) 
    {
		// A만 밀림
		if (aabb_a.minVector.y <= aabb_b.maxVector.y)
        {
            //rbA->isGrounded = true;
            // 정확하게 위에 있을때만, 이건 x축의 벽에 부딪치는 경우.
            if(!(aabb_a.minVector.x > aabb_b.maxVector.x - 20 || aabb_a.maxVector.x < aabb_b.minVector.x + 20))
                rbA->m_eRigidBodyState = Define::ERigidBodyState::Ground;
        }
		else
		{
            rbA->m_eRigidBodyState = Define::ERigidBodyState::Space;
			//rbA->isGrounded = false;
		}
		a->GetOwner()->transform()->AddPosition(pushX * 1.6f, pushY);
	}
	else if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic) 
    {
		// B만 밀림
		if (aabb_b.minVector.y <= aabb_a.maxVector.y)
		{
            if (!(aabb_b.minVector.x > aabb_a.maxVector.x - 20 || aabb_b.maxVector.x < aabb_a.minVector.x + 20))
                rbB->m_eRigidBodyState = Define::ERigidBodyState::Ground;
		}
		else
		{
            rbB->m_eRigidBodyState = Define::ERigidBodyState::Space;
		}
		b->GetOwner()->transform()->AddPosition(-pushX * 1.6f, -pushY);
	}
	// 나머지(Static-Static, Kinematic-Kinematic, Static-Kinematic): 아무것도 안 함
}

void Physics::FCollisionDetector::PushOverlappedAreaNoMass(Collider* a, Collider* b)
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

bool Physics::FCollisionDetector::LineAABBIntersect(const FVector2& p0, const FVector2& p1, const FAABB& aabb, FVector2& outHitPos)
{
    float tmin = 0.0f;
    float tmax = 1.0f;

    FVector2 d = p1 - p0;

    for (int axis = 0; axis < 2; ++axis)
    {
        float p = (axis == 0) ? d.x : d.y;
        float minVal = (axis == 0) ? aabb.minVector.x : aabb.minVector.y;
        float maxVal = (axis == 0) ? aabb.maxVector.x : aabb.maxVector.y;
        float p0val = (axis == 0) ? p0.x : p0.y;

        if (fabs(p) < 1e-6f) // 이동량이 거의 없는 경우
        {
            if (p0val < minVal || p0val > maxVal)
                return false; // 해당 축 범위를 벗어남
        }
        else
        {
            float t1 = (minVal - p0val) / p;
            float t2 = (maxVal - p0val) / p;

            if (t1 > t2) std::swap(t1, t2);
            tmin = max(tmin, t1);
            tmax = min(tmax, t2);
            if (tmin > tmax)
                return false;
        }
    }
    // 교차 지점 = p0 + (p1-p0) * tmin
    outHitPos = p0 + d * tmin;
    // (추가로 tmin이 0~1 구간 안에 있어야 함: 이미 위에서 보장)
    return true;
}
