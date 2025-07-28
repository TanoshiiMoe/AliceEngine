#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>

class TileMapComponent : public RenderComponent
{
public:
	TileMapComponent();
	~TileMapComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};

