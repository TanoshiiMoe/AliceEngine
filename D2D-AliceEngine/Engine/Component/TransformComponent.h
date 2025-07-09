#pragma once
#include "Component.h"
#include <Math/Transform.h>

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
	// °èÃþ±¸Á¶ °ü¸®
	WeakObjectPtr<TransformComponent> parent;
	std::vector<WeakObjectPtr<TransformComponent>> children;

	void AddChildObject(WeakObjectPtr<TransformComponent> child);

	FVector2 m_pivot{ 0,0 }; // ÁÂÇ¥ Áß½ÉÁ¡
	Transform m_localTransform; // Transform ÄÄÆ÷³ÍÆ®
	Transform m_worldTransform; // Transform ÄÄÆ÷³ÍÆ®

	FVector2 GetPosition() const;
	void SetPosition(const float& _x, const float& _y);
	void SetPosition(const float& _x);
	void SetPosition(const FVector2& _v);
	void SetRotation(const float& _val);

	float GetRotation();
	FVector2 GetScale();
	void SetScale(const float& _x, const float& _y);
	void SetScale(const float& _x);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	void SetPivot(const float& _x, const float& _y);
	void SetPivot(const float& _x);

	FVector2* GetPivot();

	void SetDirty();
};