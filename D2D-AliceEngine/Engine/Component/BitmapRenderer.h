#pragma once
#include "pch.h"
#include <Math/Transform.h>
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"

using namespace Microsoft::WRL;

class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer() {}
	~SpriteRenderer() {}

	void Initialize() override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	Transform* m_pTransform;
	FVector2* m_pivot;
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ÄÄÆ÷³ÍÆ®
};