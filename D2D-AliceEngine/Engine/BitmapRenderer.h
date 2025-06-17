#pragma once
#include "pch.h"
#include "Transform.h"
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"

using namespace Microsoft::WRL;

class BitmapRenderer : public RenderComponent
{
public:
	BitmapRenderer() {}
	~BitmapRenderer() {}

	void Initialize() override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	std::weak_ptr<Transform> m_pTransform;
	FVector2* m_pivot;
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ÄÄÆ÷³ÍÆ®
};