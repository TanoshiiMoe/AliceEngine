#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Component/Rigidbody2D.h>
#include <Component/ScriptComponent.h>
#include <System/CollisionSystem.h>
#include "Collision2D.h"

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
				std::wcout << L"[�浹] " << objects[i]->GetName()<< " " << objects[j]->GetName() << " ��ħ\n";
			}
		}
	}
}

std::unordered_set<Rigidbody2D*> Physics::FCollisionDetector::SweepAndPruneOverlapCheck(std::vector<WeakObjectPtr<Collider>>& objects)
{
    // Sweep and Prune�� ���� ����
    std::sort(objects.begin(), objects.end(), &FCollisionDetector::CompareColliderMinX);
    
    std::unordered_set<Rigidbody2D*> overlappedRigdBodies;
    CollisionSystem::GetInstance().currentCollisions.clear();
    for (size_t i = 0; i < objects.size(); ++i)
    {
        auto src = objects[i];
		if (CheckCollisionCondition(src)) continue;

        for (size_t j = i + 1; j < objects.size(); ++j)
        {
			auto tar = objects[j];
			if (CheckCollisionCondition(tar)) continue;

            // Prune: �� �̻� ��ĥ �� ������ break
            if (tar->aabb.minVector.x > src->aabb.maxVector.x)
                break;

            // ���� �浹 �˻�
            if (IsOverlapped(src, tar))
            {
				if (src.expired() || tar.expired()) continue;
                PushOverlappedArea(src.Get(), tar.Get());
				SavePreviousCollisionData(src.Get(), tar.Get(), overlappedRigdBodies);
            }
        }
    }
	LoadPreviousCollisions();

    return overlappedRigdBodies;
}

bool Physics::FCollisionDetector::CheckCollisionCondition(WeakObjectPtr<Collider>& _object)
{
	if (!_object) return true;
	if (_object->dirty) return true;
	if (_object->GetOwner()->transform()->bMoved == false) return true;
	return false;
}

void Physics::FCollisionDetector::LoadPreviousCollisions()
{
	// ���� �浹 ������ ���� �˻��ϸ鼭 ���� �浹������ ���ٸ� �װ� Exit�̴�.
	for (const auto& pair : CollisionSystem::GetInstance().previousCollisions)
	{
		if (CollisionSystem::GetInstance().currentCollisions.find(pair) == CollisionSystem::GetInstance().currentCollisions.end())
		{
			Collider* a = pair.first;
			Collider* b = pair.second;
			Collision2D collision2D;
			collision2D.collider = a;
			collision2D.otherCollider = b;
			if (a->GetOwner()) collision2D.rigidbody = a->GetOwner()->GetComponent<Rigidbody2D>();
			if (b->GetOwner()) collision2D.otherRigidbody = b->GetOwner()->GetComponent<Rigidbody2D>();
			collision2D.transform = a->GetTransform();

			std::vector<ScriptComponent*> scA, scB;
			if (a->GetOwner()) scA = a->GetOwner()->GetComponents<ScriptComponent>();
			if (b->GetOwner()) scB = b->GetOwner()->GetComponents<ScriptComponent>();
			for (auto sc : scA) sc->OnCollisionExit2D(&collision2D);
			for (auto sc : scB) sc->OnCollisionExit2D(&collision2D);
		}
	}
	CollisionSystem::GetInstance().previousCollisions = CollisionSystem::GetInstance().currentCollisions;
}

void Physics::FCollisionDetector::SavePreviousCollisionData(Collider* src, Collider* tar, std::unordered_set<Rigidbody2D*>& overlappedRigdBodies)
{
	// ����� ������ ������� ������.
	Collider* a = src;
	Collider* b = tar;
	Rigidbody2D* rbA = src->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = tar->GetOwner()->GetComponent<Rigidbody2D>();
	std::vector<ScriptComponent*> scA = a->GetOwner()->GetComponents<ScriptComponent>();
	std::vector<ScriptComponent*> scB = b->GetOwner()->GetComponents<ScriptComponent>();

	auto pair = std::minmax(a, b);
	CollisionSystem::GetInstance().currentCollisions.insert(pair);

	if (rbA) overlappedRigdBodies.insert(rbA);
	if (rbB) overlappedRigdBodies.insert(rbB);

	// �浹�� �Ͼ ������ ��Ƽ� �ǳ�����.
	Collision2D collision2D;
	collision2D.collider = a;
	collision2D.otherCollider = b;
	collision2D.rigidbody = rbA;
	collision2D.otherRigidbody = rbB;
	collision2D.transform = a->GetTransform();

	// ���� �浹������ ���ؼ� Enter���� Stay���� ��������.
	if (CollisionSystem::GetInstance().previousCollisions.find(pair) == CollisionSystem::GetInstance().previousCollisions.end())
	{
		for (auto sc : scA) sc->OnCollisionEnter2D(&collision2D);
		for (auto sc : scB) sc->OnCollisionEnter2D(&collision2D);
	}
	else
	{
		for (auto sc : scA) sc->OnCollisionStay2D(&collision2D);
		for (auto sc : scB) sc->OnCollisionStay2D(&collision2D);
	}
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

// �浹�� �����ϰ� AABB ������ �� �κ��� �о�� �����Դϴ�.
// �� ���¸� ������ �����մϴ�. 
// 1. �� ��
// 2. �� ���� �ִ� ������Ʈ�� ��
// 3. ���� ��
//
// --- �̹� ������ ���� �Լ� �״�� ��� ---
// CalcXOverlap, CalcYOverlap, Width, IsOnGround

void Physics::FCollisionDetector::PushOverlappedArea(Collider* a, Collider* b)
{
	FAABB& aabb_a = a->aabb;
	FAABB& aabb_b = b->aabb;
	Rigidbody2D* rbA = a->GetOwner()->GetComponent<Rigidbody2D>();
	Rigidbody2D* rbB = b->GetOwner()->GetComponent<Rigidbody2D>();
	float massA = rbA ? rbA->mass : FLT_MAX;
	float massB = rbB ? rbB->mass : FLT_MAX;
	Define::ERigidBodyType typeA = rbA ? rbA->m_eRigidBodyType : Define::ERigidBodyType::Static;
	Define::ERigidBodyType typeB = rbB ? rbB->m_eRigidBodyType : Define::ERigidBodyType::Static;

	// ��ģ �Ÿ� ��� (�Լ� ���)
	float overlap_x = CalcXOverlap(aabb_a, aabb_b);
	float overlap_y = CalcYOverlap(aabb_a, aabb_b);

	if (overlap_x <= Define::MIN_OVERLAP_EPSILON || overlap_y <= Define::MIN_OVERLAP_EPSILON)
		return;

	// �и� ���� ����
	float pushX = 0, pushY = 0;
	if (overlap_x < overlap_y)
	{
		pushX = (aabb_a.minVector.x < aabb_b.minVector.x) ? -overlap_x : overlap_x;
		pushY = 0;
	}
	else
	{
		pushY = (aabb_a.minVector.y < aabb_b.minVector.y) ? -overlap_y : overlap_y;
		pushX = 0;
	}

	constexpr float GROUND_OVERLAP_X_THRESHOLD = 0.3f;

	float totalMass = massA + massB;
	float ratioA = massA / totalMass;
	float ratioB = massB / totalMass;

	if (typeA == Define::ERigidBodyType::Dynamic && typeB == Define::ERigidBodyType::Dynamic)
	{
		// �߽��� ���
		float centerA_y = (aabb_a.minVector.y + aabb_a.maxVector.y) * 0.5f;
		float centerB_y = (aabb_b.minVector.y + aabb_b.maxVector.y) * 0.5f;

		Rigidbody2D* upper = nullptr;
		Rigidbody2D* lower = nullptr;

		// y�� ���� ��/�Ʒ� �Ǻ�
		if (centerA_y > centerB_y) {
			upper = rbA;
			lower = rbB;
		}
		else {
			upper = rbB;
			lower = rbA;
		}

		// upper(���� �ִ� ������Ʈ) ���� ���� (���� �״��)
		switch (upper->m_eRigidBodyState)
		{
		case Define::ERigidBodyState::Space:
			switch (lower->m_eRigidBodyState)
			{
			case Define::ERigidBodyState::Space:
				break;
			case Define::ERigidBodyState::Ground:
			case Define::ERigidBodyState::OnRigidBody:
				upper->m_eRigidBodyState = Define::ERigidBodyState::OnRigidBody;
				break;
			}
			break;
		case Define::ERigidBodyState::Ground:
			break;
		case Define::ERigidBodyState::OnRigidBody:
			switch (lower->m_eRigidBodyState)
			{
			case Define::ERigidBodyState::Space:
				upper->m_eRigidBodyState = Define::ERigidBodyState::Space;
				break;
			case Define::ERigidBodyState::Ground:
				break;
			case Define::ERigidBodyState::OnRigidBody:
				break;
			}
			break;
		}

		// �� �� �и� (���� ����)
		if (rbA) rbA->collisionPush += FVector2(pushX * ratioA * 1.2f, pushY * ratioA);
		if (rbB) rbB->collisionPush += FVector2(-pushX * ratioB * 1.2f, -pushY * ratioB);
	}
	else if (typeA == Define::ERigidBodyType::Dynamic && (typeB == Define::ERigidBodyType::Static || typeB == Define::ERigidBodyType::Kinematic))
	{
		bool is_y_push = (pushY != 0);  // y������ �и��� ���� ����
		bool is_on_ground = false;
		if (is_y_push && aabb_a.minVector.y <= aabb_b.maxVector.y) 
		{
			// ������ y������ �Ʒ�(��, �ٴڿ�) ��Ұ�, x�� ��ħ ������ ����� ����
			is_on_ground = IsOnGround(aabb_a, aabb_b, GROUND_OVERLAP_X_THRESHOLD);
		}
		if (rbA) rbA->m_eRigidBodyState = is_on_ground ? Define::ERigidBodyState::Ground : Define::ERigidBodyState::Space;
		if (rbA) rbA->collisionPush += FVector2(pushX * 1.5f, pushY);
	}
	else if ((typeA == Define::ERigidBodyType::Static || typeA == Define::ERigidBodyType::Kinematic) && typeB == Define::ERigidBodyType::Dynamic)
	{
		bool is_y_push = (pushY != 0);
		bool is_on_ground = false;
		if (is_y_push && aabb_b.minVector.y <= aabb_a.maxVector.y) 
		{
			is_on_ground = IsOnGround(aabb_b, aabb_a, GROUND_OVERLAP_X_THRESHOLD);
		}
		if (rbB) rbB->m_eRigidBodyState = is_on_ground ? Define::ERigidBodyState::Ground : Define::ERigidBodyState::Space;
		if (rbB) rbB->collisionPush += FVector2(-pushX * 1.5f, -pushY);
	}
	// ������(Static-Static, Kinematic-Kinematic, Static-Kinematic): �ƹ��͵� �� ��
}


void Physics::FCollisionDetector::PushOverlappedAreaNoMass(Collider* a, Collider* b)
{
    // aabb ���� ��������
    const auto& aabb_a = a->aabb;
    const auto& aabb_b = b->aabb;

    // ��ģ �Ÿ� ���
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

        if (fabs(p) < 1e-6f) // �̵����� ���� ���� ���
        {
            if (p0val < minVal || p0val > maxVal)
                return false; // �ش� �� ������ ���
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
    // ���� ���� = p0 + (p1-p0) * tmin
    outHitPos = p0 + d * tmin;
    // (�߰��� tmin�� 0~1 ���� �ȿ� �־�� ��: �̹� ������ ����)
    return true;
}

float Physics::FCollisionDetector::CalcXOverlap(const FAABB& a, const FAABB& b)
{
    return min(a.maxVector.x, b.maxVector.x) - max(a.minVector.x, b.minVector.x);
}

float Physics::FCollisionDetector::CalcYOverlap(const FAABB& a, const FAABB& b)
{
    return min(a.maxVector.y, b.maxVector.y) - max(a.minVector.y, b.minVector.y);
}

float Physics::FCollisionDetector::Width(const FAABB& box)
{
    return box.maxVector.x - box.minVector.x;
}

bool Physics::FCollisionDetector::IsOnGround(const FAABB& above, const FAABB& below, float threshold)
{
	float x_overlap = CalcXOverlap(above, below);
	float overlap_ratio_above = x_overlap / Width(above);
	float overlap_ratio_below = x_overlap / Width(below);
	return (overlap_ratio_above > threshold) || (overlap_ratio_below > threshold);
}
