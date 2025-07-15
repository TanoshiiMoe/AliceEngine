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

	virtual float GetSizeX() = 0;
	virtual float GetSizeY() = 0;
	virtual void Render() override;

	Define::EDrawType drawType = Define::EDrawType::WorldSpace;
	int m_layer = -999;
protected:
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();
};

