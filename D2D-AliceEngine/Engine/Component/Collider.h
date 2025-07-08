#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>

/*
*	@brief : 충돌을 감지하는 컴포넌트입니다.
*	@details : SetAABBBoxSize() 함수를 실행해주어야 크기에 맞는 AABB 박스가 생깁니다.
*/

class Collider : public Component
{
public:
	Collider();
	~Collider();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	void SetAABBBoxSize(const FVector2& minVector, const FVector2& maxVector);

	bool bShowColliderBox = false;
	FAABB aabb;
};