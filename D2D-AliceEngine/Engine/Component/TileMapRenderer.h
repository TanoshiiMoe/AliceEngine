#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>

struct TileMap;
struct TileSet;
class TileMapRenderer : public RenderComponent
{
public:
	TileMapRenderer();
	~TileMapRenderer();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	std::vector<int> tileData;
	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	ComPtr<ID2D1Effect> m_effect; // 이펙트

	struct FSlicedArea {
		float srcX{ 0 }, srcY{ 0 };
		float srcW{ -1 }, srcH{ -1 };
	} slice;

	int mapWidth = 0;
	int mapHeight = 0;
	int tileWidth = 0;			// tileWidth
	int tileHeight = 0;			// tileHeight
	int tilesetColumns = 0;
	int gid = 0;

	SpriteInfo spriteInfo; // 스프라이트 정보

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	void SetMapInfo(const TileMap& mapInfo, const TileSet& setInfo);
	void SetSlice(float x, float y, float w, float h);
	void SetSkew(bool _setActive, FVector2 _skewAngle = FVector2(0.0f, 0.0f));

private:
	TransformComponent* m_transform = nullptr;
	FVector2 skewAngle{ 30 };

	ComPtr<ID2D1Bitmap1> slicedBitmap;
	ComPtr<ID2D1Bitmap1> GetSlicedBitmap(ID2D1Bitmap1* bitmap, const D2D1_RECT_F& srcRect);
	D2D1_MATRIX_3X2_F CreateTileTransform(float x, float y, float skewX, float skewY, const D2D1_MATRIX_3X2_F& viewMatrix);
};

