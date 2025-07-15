#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>
#include <Define/Define.h>
#include <Experimental/Collision/CollisionDetector.h>

Rigidbody2D::Rigidbody2D()
{

}

Rigidbody2D::~Rigidbody2D()
{
	PhysicsSystem::GetInstance().UnRegist(WeakFromThis<Rigidbody2D>());
}

void Rigidbody2D::Initialize()
{
	PhysicsSystem::GetInstance().Regist(WeakFromThis<Rigidbody2D>());
}

void Rigidbody2D::Update(const float& deltaSeconds)
{
	float substep = 0.016f;
	float remaining = deltaSeconds;
	while (remaining > 0.0f)
	{
		float dt = min(substep, remaining);

		// 1. 중력 힘 추가
		if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
		{
			FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
			force += gravityForce;
		}

		// 2. 전체 가속도 계산
		FVector2 acceleration = force / mass;

		// 3. 속도 갱신
		velocity += acceleration * dt;

		// 4. 감쇠(마찰) 적용
		velocity.x *= (1.0f - drag * dt);
		velocity.y *= (1.0f - drag * dt);

		// 5. 위치 갱신
		owner->transform()->AddPosition(velocity.x * dt, velocity.y * dt);
		owner->transform()->AddRotation(angularVelocity * dt);


		// ========== x축 터널링 방지 시작 ==========
		// 이동 전 현재 위치
		FVector2 prevPos = owner->transform()->GetPosition();
		// 이동 예정 위치 (x축)
		FVector2 nextPos = prevPos + FVector2(velocity.x * dt, 0);

		bool blockedX = false;
		float minHitT = 1.0f; // 0~1, 충돌 타이밍
		FVector2 hitPos;

		// (씬 내 모든 Collider 대상으로 sweep)
		for (auto& weakRb : CollisionSystem::GetInstance().m_colliders)
		{
			if (auto sp = weakRb.lock())
			{
				if (!sp || sp == owner->GetComponent<Collider>()) continue;
				// 선분-박스 교차 검사 (여기서는 단순화 예시, AABB Sweep을 쓸 수도 있음)
				if (Physics::FCollisionDetector::LineAABBIntersect(prevPos, nextPos, sp->aabb, hitPos))
				{
					// (t = prevPos ~ nextPos의 비율, 0~1)
					float dx = nextPos.x - prevPos.x;
					float t = dx != 0 ? (hitPos.x - prevPos.x) / dx : 1.0f;
					if (t < minHitT)
					{
						minHitT = t;
						blockedX = true;
					}
				}
			}
		}

		if (blockedX)
		{
			// 충돌 위치까지 이동 (t 비율만큼)
			float xMove = (velocity.x * dt) * minHitT;
			owner->transform()->AddPosition(xMove, 0);
			velocity.x = 0.0f; // 정지
		}
		else
		{
			owner->transform()->AddPosition(velocity.x * dt, 0);
		}

		// ========== x축 터널링 방지 끝 ==========

		// 6. 힘 초기화
		force = FVector2(0.0f, 0.0f);
		torque = 0.0f;

		// 7. 상태별 y속도 제한 (Ground, OnRigidBody에서만 아래로 가려는 힘 막음)
		if (m_eRigidBodyState == Define::ERigidBodyState::Ground ||
			m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody)
		{
			if (velocity.y < 0)
				velocity.y = 0.0f;
		}

		remaining -= dt;
	}
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x += _x * 1000;
	force.y += _y * 1000;
}