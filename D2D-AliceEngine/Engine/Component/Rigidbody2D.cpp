#include "pch.h"
#include "Rigidbody2D.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <System/PhysicsSystem.h>

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
	//FVector2 pos = owner->transform()->GetPosition();
	// 1. 중력 적용 (isGrounded가 아니면)
	if (!isGrounded && m_eRigidBodyType != Define::ERigidBodyType::Static)
	{
		// F = m * a, 중력가속도는 대략 9.8, 여기선 -12로 가정
		FVector2 gravityForce(0, -9.8f * gravityScale * mass);
		force += gravityForce;
	}

	// 2. 가속도 계산 (a = F/m)
	FVector2 acceleration = force / mass;

	// 3. 속도 업데이트 (v = v0 + at)
	velocity += acceleration * deltaSeconds;

	// 4. 감쇠 적용
	velocity -= velocity * drag * deltaSeconds;
	angularVelocity -= angularVelocity * angularDrag * deltaSeconds;

	// 5. 위치, 회전 업데이트
	owner->transform()->AddPosition(velocity.x * deltaSeconds, velocity.y * deltaSeconds);
	owner->transform()->AddRotation(angularVelocity * deltaSeconds);

	// 6. 힘/토크 초기화 (프레임마다)
	force = FVector2(0.0f, 0.0f);
	torque = 0.0f;

	// 7. 충돌 처리: 땅에 닿았으면 y속도 0, isGrounded true
	if (isGrounded)
	{
		velocity.y = 0.0f;
	}
}

void Rigidbody2D::Release()
{
}

void Rigidbody2D::AddForce(const float& _x, const float& _y)
{
	force.x = _x;
	force.y = _y;
}