#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>

/*
*	@brief : 충돌을 감지하는 컴포넌트입니다.
*	@details : SetAABBBoxSize() 함수를 실행해주어야 크기에 맞는 AABB 박스가 생깁니다.
*/
class BoxComponent;
class Collider : public Component
{
public:
	Collider();
	~Collider();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	void SetBoxSize(const FVector2& _size);
	void SetBoxPosition(const FVector2& _pos);
	void SetBoxColor(const FColor _color);
	void UpdateAABB();

	FAABB aabb;
	BoxComponent* boxComponent;

	bool dirty = true;
	float collisionDelay = 0.1f;
};