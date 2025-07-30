#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>
#include <Define/Define.h>
#include <Experimental/Collision/CollisionDetector.h>
#include <Manager/TimerManager.h>

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
	m_prevPosition = m_currentPosition;
	m_currentPosition = owner->transform()->GetPosition();
	m_prevRotation = m_currentRotation = owner->transform()->GetRotation();
}

void Rigidbody2D::Update(const float& deltaSeconds)
{
	if (auto col = GetOwner()->GetComponent<Collider>())
		if (col->dirty) return;

	if (!GetOwner()->transform()->bMoved)
		return;

	// FixedUpdate ���� �� ���� ��ġ ����
	m_prevPosition = m_currentPosition;
	m_prevRotation = m_currentRotation;

	const float dt = deltaSeconds;
	FVector2 calcPos = owner->transform()->GetPosition() + collisionPush;
	collisionPush = FVector2(0, 0);
	FVector2 calcVel = velocity;
	float calcAngle = owner->transform()->GetRotation();
	float calcAngularVel = angularVelocity;
	FVector2 calcForce = force;
	float calcTorque = torque;

		// �߷�
	if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
	{
		FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
		calcForce += gravityForce;
	}

	FVector2 acceleration = calcForce / mass;
	calcVel += acceleration * dt;
	calcVel.x *= (1.0f - drag * dt);
	calcVel.y *= (1.0f - drag * dt);

		// y�� �̵�
	calcPos.y += calcVel.y * dt;
	calcAngle += calcAngularVel * dt;

		// x�� �ͳθ� ���� �˰��� calcPos ��������!
	FVector2 prevPos = calcPos;
	FVector2 nextPos = prevPos + FVector2(calcVel.x * dt, 0);
	bool blockedX = false;
	float minHitT = 1.0f;
	FVector2 hitPos;

	for (auto& weakRb : CollisionSystem::GetInstance().m_colliders)
	{
		if (auto sp = weakRb.lock())
		{
			if (!sp || sp == owner->GetComponent<Collider>()) continue;

			if (Physics::FCollisionDetector::LineAABBIntersect(prevPos, nextPos, sp->aabb, hitPos))
			{
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
		calcPos.x += (calcVel.x * dt) * minHitT;
		calcVel.x = 0.0f;
	}
	else
	{
		calcPos.x += calcVel.x * dt;
	}

	// �ٿ ����
	if ((m_eRigidBodyState == Define::ERigidBodyState::Ground ||
		m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody) &&
		calcVel.y < 0)
	{
		calcVel.y = 0.0f;
	}

	// ���� ����
	 // ���� ��� �Ϸ� �� ���� ��ġ ����
    m_currentPosition = calcPos;
    m_currentRotation = calcAngle;

	owner->transform()->SetPosition(calcPos);
	owner->transform()->SetRotation(calcAngle);
	velocity = calcVel;
	angularVelocity = calcAngularVel;

	// ���� (CollisionSystem ��) �����Ӵ� 1ȸ�� Update
	force = FVector2(0.0f, 0.0f);
	torque = 0.0f;
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x += _x * 1000;
	force.y += _y * 1000;
}

FVector2 Rigidbody2D::GetInterpolatedPosition() const
{
	float alpha = TimerManager::GetInstance().GetAccumulator() / TimerManager::GetInstance().fixedDeltaTime;
	alpha = Math::clamp(alpha, 0.0f, 1.0f);

	// ���� ����: prevPos + (currPos - prevPos) * alpha
	return FVector2(
		m_prevPosition.x + (m_currentPosition.x - m_prevPosition.x) * alpha,
		m_prevPosition.y + (m_currentPosition.y - m_prevPosition.y) * alpha
	);
}

float Rigidbody2D::GetInterpolatedRotation() const
{
	float alpha = TimerManager::GetInstance().GetAccumulator() / TimerManager::GetInstance().fixedDeltaTime;
	alpha = Math::clamp(alpha, 0.0f, 1.0f);

	return m_prevRotation + (m_currentRotation - m_prevRotation) * alpha;
}