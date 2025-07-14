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
	Define::ERigidBodyType m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
};