#pragma once

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <wrl/client.h>
#include <memory>

#include "Component.h"
#include <Define/Define.h>

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();
public:
	void Initialize() override;
	void Update() override;
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
	D2D1::Matrix3x2F view;
	FVector2 scale{ 1.0f, 1.0f };
};

