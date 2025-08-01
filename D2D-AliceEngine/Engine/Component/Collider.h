#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>

/*
*	@brief : �浹�� �����ϴ� ������Ʈ�Դϴ�.
*	@details : SetAABBBoxSize() �Լ��� �������־�� ũ�⿡ �´� AABB �ڽ��� ����ϴ�.
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
	void SetBoxColor(const FColor _color);
	void UpdateAABB();

	FAABB aabb;
	BoxComponent* boxComponent;

	bool dirty = true;
	float collisionDelay = 0.1f;
};