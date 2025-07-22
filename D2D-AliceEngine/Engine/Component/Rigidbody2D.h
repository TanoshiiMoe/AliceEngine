#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>
#include <Define/Define.h>

/*
*	@brief : ������ �ùķ��̼��ϴ� ������Ʈ�Դϴ�.
*	@details : ERigidBodyState�� ���¸� �����մϴ�.
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

	// <���� �Ӽ� ������>
	FVector2 velocity = FVector2(0.0f, 0.0f);           // ���ӵ�
	float angularVelocity = 0.0f;                     // ���ӵ�
	float mass = 1.0f;                                // ����
	float drag = 0.25f;                                // ���� ���� (�� ���� ����)
	float angularDrag = 0.0f;                        // �� ���� (0~1 ����, �⺻ 0)
	FVector2 force = FVector2(0.0f, 0.0f);              // ���� ��
	float torque = 0.0f;                              // ���� ��ũ
	float gravityScale = 1.0f;                        // �߷� ���� ��� (���� ����)
	Define::ERigidBodyState m_eRigidBodyState = Define::ERigidBodyState::Space; // ���� ��Ҵ��� ����
	Define::ERigidBodyType m_eRigidBodyType = Define::ERigidBodyType::Kinematic;

	FVector2 collisionPush;

	void AddForce(const float& _x, const float& _y);
};