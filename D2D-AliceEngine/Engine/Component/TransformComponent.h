#pragma once
#include "Component.h"

class Transform;
class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();
public:
	void Initialize() override;
	void Release() override;
	void Update() override;

	void SetTransform(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

public:
	// °èÃþ±¸Á¶ °ü¸®
	std::weak_ptr<TransformComponent> parent;
	std::vector<std::weak_ptr<TransformComponent>> children;

	void AddChildObject(std::weak_ptr<TransformComponent> child);

	FVector2 m_pivot{ 0,0 }; // ÁÂÇ¥ Áß½ÉÁ¡
	Transform* m_localTransform; // Transform ÄÄÆ÷³ÍÆ®
	Transform* m_worldTransform; // Transform ÄÄÆ÷³ÍÆ®

	void SetPosition(const float& _x, const float& _y);
	void SetPosition(const float& _x);
	void SetRotation(const float& _val);

	void SetScale(const float& _x, const float& _y);
	void SetScale(const float& _x);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	void SetPivot(const float& _x, const float& _y);
	void SetPivot(const float& _x);

	FVector2* GetPivot();

	void SetDirty();
};