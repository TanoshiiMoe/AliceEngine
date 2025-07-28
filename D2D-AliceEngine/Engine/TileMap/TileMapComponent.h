#pragma once
#include <Component/RenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <TileMap/TileMapLoader.h>

struct TileMapWrapper
{
	int row, col; // Ÿ�ϸ��� ��� ��
	int width, height; // Ÿ�ϸ��� �ʺ�� ����
	WeakObjectPtr<SpriteRenderer> spriteRenderer; // Ÿ�Ͽ� �ش��ϴ� ��������Ʈ ������
};

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
	std::vector<WeakObjectPtr<TileMapWrapper>> m_TileRenderers;
	TileMap tilemap;
	TileSet tileset;
};

