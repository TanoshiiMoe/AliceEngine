#pragma once
#include "Component.h"
#include <System/RenderSystem.h>
#include <Math/Transform.h>
#include <Math/TMath.h>

class RenderComponent : public Component
{
public:
	RenderComponent() { m_pTransform = nullptr; m_pivot = nullptr; }
	~RenderComponent() { RenderSystem::Get().UnRegist(this->weak_from_this()); }
public:
	void Initialize() override;
	void Update() override;
	void Release() override;

	virtual void Render() = 0;

	// TransformComponent의 worlTransform의 주소값이 들어갑니다.
	// 새로 할당하지 않습니다.
	Transform* m_pTransform;
	FVector2* m_pivot;
};

