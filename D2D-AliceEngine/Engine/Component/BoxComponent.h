#pragma once
#include "Component/RenderComponent.h"
#include <Math/TColor.h>
#include <Math/TMath.h>

class BoxComponent : public RenderComponent
{
public:
	BoxComponent();
	BoxComponent(const FVector2& _size, const FColor& color);
	~BoxComponent();

	void Initialize() override;
	void Release() override;
	void Render() override;

	void SetSize(const FVector2& _size)
	{
		m_size = _size;		
	}

	void SetColor(const FColor& color);
	FColor GetColor() { return m_color; }

	ComPtr<ID2D1SolidColorBrush> m_pBrush;
	FColor m_color;
	FVector2 m_size;
};

