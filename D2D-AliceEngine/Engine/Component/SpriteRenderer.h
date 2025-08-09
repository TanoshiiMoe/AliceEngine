#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>

using namespace Microsoft::WRL;

struct ID2D1Bitmap1;
struct ID2D1Effect;
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

	FVector2 GetBitmapSize();
	FVector2 GetRelativeSize();

	void SetSlice(float x, float y, float w, float h);
	void SetSkewing(bool _isSkewing, FVector2 _skewAngle = FVector2(0.0f, 0.0f));

	// Effect helpers for transitions
	void SetOpacity(float alpha); // 0..1
	void ClearEffect() { m_effect.Reset(); }

	SpriteInfo spriteInfo; // ��������Ʈ ����

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	ComPtr<ID2D1Effect> m_effect;	// ����Ʈ �̹���

	struct FSlicedArea {
		float srcX{ 0 }, srcY{ 0 };
		float srcW{ -1 }, srcH{ -1 };
	} slice;
};