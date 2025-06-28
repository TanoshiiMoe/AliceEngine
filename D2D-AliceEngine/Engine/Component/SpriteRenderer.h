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
	~SpriteRenderer();

	void Initialize() override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	FVector2 GetSize()
	{
		D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
		return FVector2(bmpSize.width, bmpSize.height);
	}

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};