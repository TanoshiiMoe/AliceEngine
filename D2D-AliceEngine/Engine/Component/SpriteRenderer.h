#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>

using namespace Microsoft::WRL;

struct ID2D1Bitmap1;
class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();

	void SetSlice(float x, float y, float w, float h);

	SpriteInfo spriteInfo; // 스프라이트 정보

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	ComPtr<ID2D1Effect> m_effect;	// 이펙트 이미지

	struct FSlicedArea {
		float srcX{ 0 }, srcY{ 0 };
		float srcW{ -1 }, srcH{ -1 };
	} slice;
};