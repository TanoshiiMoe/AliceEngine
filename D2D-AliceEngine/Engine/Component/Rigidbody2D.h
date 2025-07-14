#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>
#include <Define/Define.h>

/*
*	@brief : 물리를 시뮬레이션하는 컴포넌트입니다.
*/
class Rigidbody2D : public Component
{
public:
	Rigidbody2D();
	~Rigidbody2D();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	// 물리 속성 변수들
	FVector2 velocity = FVector2(0.0f, 0.0f);           // 선속도
	float angularVelocity = 0.0f;                     // 각속도
	float mass = 1.0f;                                // 질량
	float drag = 0.0f;                                // 선형 감쇠
	float angularDrag = 0.05f;                        // 각 감쇠
	FVector2 force = FVector2(0.0f, 0.0f);              // 누적 힘
	float torque = 0.0f;                              // 누적 토크
	float gravityScale = 1.0f;                        // 중력 영향 배수
	bool isGrounded = false; // 땅에 닿았는지 여부

	Define::ERigidBodyType m_eRigidBodyType = Define::ERigidBodyType::Kinematic;

	void AddForce(const float& _x, const float& _y);
};