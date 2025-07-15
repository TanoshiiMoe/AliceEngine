#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>
#include <Define/Define.h>

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

		if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
		{
			FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
			force += gravityForce;
		}

		FVector2 acceleration = force / mass;
		velocity += acceleration * dt;

		velocity.x *= (1.0f - drag * dt);
		velocity.y *= (1.0f - drag * dt);
		angularVelocity *= (1.0f - angularDrag * dt);

		owner->transform()->AddPosition(velocity.x * dt, velocity.y * dt);
		owner->transform()->AddRotation(angularVelocity * dt);

		force = FVector2(0.0f, 0.0f);
		torque = 0.0f;

		if (m_eRigidBodyState != Define::ERigidBodyState::Space)
		{
			velocity.y = 0.0f;
		}

		remaining -= dt;
	}

	/*if (m_eRigidBodyType == Define::ERigidBodyType::Dynamic)
	{
		FVector2 gravityForce(0, -Define::GRAVITYCONSTANT * gravityScale * mass);
		force += gravityForce;
	}

	FVector2 acceleration = force / mass;
	velocity += acceleration * deltaSeconds;

	velocity.x *= (1.0f - drag * deltaSeconds);
	velocity.y *= (1.0f - drag * deltaSeconds);
	angularVelocity *= (1.0f - angularDrag * deltaSeconds);
	
	owner->transform()->AddPosition(velocity.x * deltaSeconds, velocity.y * deltaSeconds);
	owner->transform()->AddRotation(angularVelocity * deltaSeconds);

	force = FVector2(0.0f, 0.0f);
	torque = 0.0f;	

	if (isGrounded)
		velocity.y = 0.0f;*/
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	m_eRigidBodyState = Define::ERigidBodyState::Space;
   /* velocity.x += _x;
    velocity.y += _y;*/
	force.x += _x * 1000;
	force.y += _y * 1000;
}