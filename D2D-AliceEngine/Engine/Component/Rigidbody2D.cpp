#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>
#include <Component/Collider.h>
#include <System/CollisionSystem.h>

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
	while (remaining > 0.0f) {
		float dt = min(substep, remaining);

		if (m_eRigidBodyType != Define::ERigidBodyType::Static)
		{
			FVector2 gravityForce(0, -9.8f * gravityScale * mass);
			if (velocity.y < 0) gravityForce *= fallMultiplier;
			force += gravityForce;
		}

		FVector2 acceleration = force / mass;
		velocity += acceleration * dt;

		velocity.x *= (1.0f - drag * dt);
		velocity.y *= (1.0f - drag * dt);
		angularVelocity *= (1.0f - angularDrag * dt);

		FVector2 moveDelta(velocity.x * dt, velocity.y * dt);
		FVector2 curPos = owner->transform()->GetPosition();
		FVector2 nextPos = curPos + moveDelta;

		bool xBisection = false, yBisection = false;
		float newX = nextPos.x;
		float newY = nextPos.y;
		const float minStep = 0.01f;

		// y축 이분법 (바닥 y=0)
		if (moveDelta.y < 0 && nextPos.y < 0) {
			float left = curPos.y;
			float right = nextPos.y;
			float mid = 0.0f;
			for (int i = 0; i < 10; ++i) {
				mid = (left + right) * 0.5f;
				if (mid < 0) right = mid;
				else left = mid;
				if (fabs(right - left) < minStep) break;
			}
			newY = 0.0f;
			yBisection = true;
		}
		// x축 이분법 (왼쪽 벽 x=0)
		if (moveDelta.x < 0 && nextPos.x < 0) {
			float left = curPos.x;
			float right = nextPos.x;
			float mid = 0.0f;
			for (int i = 0; i < 10; ++i) {
				mid = (left + right) * 0.5f;
				if (mid < 0) right = mid;
				else left = mid;
				if (fabs(right - left) < minStep) break;
			}
			newX = 0.0f;
			xBisection = true;
		}
		// (오른쪽/위쪽 경계 필요시 여기에 추가)

		if (xBisection || yBisection) {
			owner->transform()->SetPosition(newX, newY);
			if (yBisection) {
				isGrounded = true;
				velocity.y = 0.0f;
			}
			if (xBisection) {
				velocity.x = 0.0f;
			}
		} else {
			owner->transform()->SetPosition(nextPos.x, nextPos.y);
		}
		owner->transform()->AddRotation(angularVelocity * dt);

		force = FVector2(0.0f, 0.0f);
		torque = 0.0f;

		if (isGrounded) 
			velocity.y = 0.0f;

		remaining -= dt;
	}
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	isGrounded = false;
    velocity.x += _x;
    velocity.y += _y;
}