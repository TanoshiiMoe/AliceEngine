#pragma once
#include "Component.h"
#include <Define/Define.h>

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	virtual float GetBitmapSizeX() = 0;
	virtual float GetBitmapSizeY() = 0;
	virtual void Render() override;

	void SetFlip(const bool& _flip) { bFlip = _flip; }
	void SetScale(const FVector2& _scale) { scale = _scale; }
	FVector2 GetScale() const { return scale; }

	Define::EDrawType drawType = Define::EDrawType::WorldSpace;
	int m_layer = -999;
	bool bFlip = false;
protected:
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();
	FVector2 scale{ 1.0f, 1.0f };
};

