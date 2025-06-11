#pragma once
#include "pch.h"
#include "Transform.h"
#include "Interface/IRenderer.h"

using namespace Microsoft::WRL;

class BitmapRenderer : public IRenderer, public std::enable_shared_from_this<BitmapRenderer>
{
public:
	BitmapRenderer()
	{

	}
	virtual ~BitmapRenderer()
	{

	}

	void Initialize() override;
	void LoadBitmapData(const std::wstring& path);
	void Update() override;
	void Release() override;
	void Render() override;

	std::weak_ptr<Transform> m_pTransform;
	FVector2* m_pivot;
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ÄÄÆ÷³ÍÆ®
};

