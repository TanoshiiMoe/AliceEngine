#pragma once
#include "Component.h"
#include <Math/Transform.h>
#include <Core/ObjectHandler.h>

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();
public:
	void Initialize() override;
	void Release() override;
	void Update(const float& deltaSeconds) override;
	void SetTransform(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

public:
	// �������� ����
	WeakObjectPtr<TransformComponent> parent;
	std::vector<WeakObjectPtr<TransformComponent>> children;

	void AddChildObject(WeakObjectPtr<TransformComponent> child);
	void RemoveFromParent();

	FVector2 m_pivot{ 0.5f,0.5f }; // ��ǥ �߽���
	Transform m_localTransform; // Transform ������Ʈ
	Transform m_worldTransform; // Transform ������Ʈ
	bool bMoved = false; // ���� Collision�� ���̴� ����

	FVector2 GetPosition() const;
	void SetPosition(const float& _x, const float& _y);
	void SetPosition(const float& _x);
	void SetPosition(const FVector2& _v);
	void SetWorldPosition(const FVector2& _v);
	void SetRotation(const float& _val);

	float GetRotation();
	FVector2 GetScale();
	void SetScale(const FVector2& _v);
	void SetScale(const float& _x, const float& _y);
	void SetScale(const float& _x);
	void SetWorldScale(const FVector2& _v);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);
	void AddPosition(const FVector2& _v);

	void SetPivot(const float& _x, const float& _y);
	void SetPivot(const float& _x);

	FVector2* GetPivot();

	void SetDirty();

//private:
//	TransformComponent* m_parent = nullptr;
//	std::vector<TransformComponent*> m_children;
};