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

	void AddTileToSpriteBatch();

	D2D1_MATRIX_3X2_F GetSkewMatrix();

	std::vector<int> tileData;
	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	int mapWidth = 0;
	int mapHeight = 0;
	int tileWidth = 0;			// tileWidth
	int tileHeight = 0;			// tileHeight
	int tilesetColumns = 0;
	int gid = 0;

	SpriteInfo spriteInfo; // ��������Ʈ ����

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	void SetMapInfo(const TileMap& mapInfo, const TileSet& setInfo);
	void SetSkew(FVector2 _skewAngle = FVector2(0.0f, 0.0f));

private:
	TransformComponent* m_transform = nullptr;
	FVector2 skewAngle{ 30 };
};

