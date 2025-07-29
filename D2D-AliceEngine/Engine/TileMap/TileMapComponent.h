#pragma once
#include <Component/RenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <TileMap/TileMapLoader.h>

class TileMapComponent : public RenderComponent
{
public:
	TileMapComponent();
	~TileMapComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadTileMapData(const std::wstring& path);
	void LoadTileSetData(const std::wstring& path);
	void LoadMapData(const std::wstring& path);
	void CreatetileRenderers();
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();

	//WeakObjectPtr<TileMapWrapper> AddSpriteRenderer(const std::wstring& path);
	
public:
	std::vector<WeakObjectPtr<SpriteRenderer>> m_TileRenderers;
	TileMap tilemap;
	TileSet tileset;

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};

