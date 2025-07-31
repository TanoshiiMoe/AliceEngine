#include "pch.h"
#include "TileMapRenderer.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Helpers/FileHelper.h>
#include <TileMap/TileMapLoader.h>

TileMapRenderer::TileMapRenderer()
{
	spriteInfo.height = -1;
	spriteInfo.width = -1;
	spriteInfo.x = 0;
	spriteInfo.y = 0;
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
	spriteInfo.name = "";
}

TileMapRenderer::~TileMapRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // ��Ʈ�� ��ε�
}

void TileMapRenderer::Initialize()
{
	__super::Initialize();
}

void TileMapRenderer::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TileMapRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(filePath.c_str());

	spriteInfo.height = GetBitmapSizeY();
	spriteInfo.width = GetBitmapSizeX();
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;

	// ��Ʈ�� ��ü
	slicedBitmap.Reset();
}

void TileMapRenderer::Release()
{
	__super::Release();
}

void TileMapRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();
	//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get());

	// SpriteBatch
	ID2D1DeviceContext7* context = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();
	ID2D1SpriteBatch* batch = D2DRenderManager::GetInstance().m_spriteBatch.Get();
	if (!context || !batch) return;
	batch->Clear();
	const int tilePerRow = m_bitmap->GetPixelSize().width / tileWidth;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int index = y * mapWidth + x;
			int tileId = tileData[index];
			if (tileId == 0)
				continue;

			tileId -= 1;
			int sx = (tileId % tilePerRow) * tileWidth ;
			int sy = (tileId / tilePerRow) * tileHeight;
			D2D1_RECT_F destRect = D2D1::RectF(
				(FLOAT)(x * tileWidth),     // X�� ������ �߰�
				(FLOAT)(y * tileHeight),                        // Y�� �״��
				(FLOAT)((x + 1) * tileWidth), // �����ʵ� ���� ������
				(FLOAT)((y + 1) * tileHeight)
			);
			D2D1_RECT_U srcRect = { (UINT32)sx, (UINT32)sy, (UINT32)(sx + tileWidth), (UINT32)(sy + tileHeight) };
			HRESULT hr = D2DRenderManager::GetInstance().m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, nullptr);
			assert(SUCCEEDED(hr));
		}
	}
	int batchSize = batch->GetSpriteCount();
	assert(batchSize > 0);
	
	float angleDeg = skewAngle.x; // ��: 30��
	float angleRad = angleDeg * (Define::PI / 180.0f);

	float totalHeight = mapHeight * tileHeight;
	float xOffset = totalHeight * std::tan(angleRad);

	// ������: ���� �Ʒ� (�پ� �ְ� �Ϸ��� ���� �������� ������)
	D2D1_MATRIX_3X2_F skewMatrix = D2D1::Matrix3x2F::Skew(angleDeg, 0);

	// ���� �Ʒ� ����, ������ ���� �̵�
	D2D1_MATRIX_3X2_F originTranslate = D2D1::Matrix3x2F::Translation(0, 0);
	D2D1_MATRIX_3X2_F skewTransform =
		originTranslate *
		skewMatrix *
		D2D1::Matrix3x2F::Translation(xOffset, 0);
	D2D1_MATRIX_3X2_F backD2DTransform = D2D1::Matrix3x2F::Translation(-Define::SCREEN_WIDTH/2, -Define::SCREEN_HEIGHT/2);
	// ���� View�� ����
	context->SetTransform(backD2DTransform *skewTransform * view);

	context->DrawSpriteBatch(
		batch,
		0, batchSize,
		m_bitmap.get(),
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1_SPRITE_OPTIONS_NONE);
}

float TileMapRenderer::GetBitmapSizeX()
{
	return static_cast<float>(mapWidth * tileWidth);
	//return 0;
}

float TileMapRenderer::GetBitmapSizeY()
{
	return static_cast<float>(mapHeight * tileHeight);
	//return 0;
}

void TileMapRenderer::SetMapInfo(const TileMap& mapInfo, const TileSet& setInfo)
{
	mapHeight = mapInfo.layers[0].height;
	mapWidth = mapInfo.layers[0].width;
	tileWidth = setInfo.tilewidth;
	tileHeight = setInfo.tileheight;
	tileData = mapInfo.layers[0].data;
}

void TileMapRenderer::SetSlice(float x, float y, float w, float h)
{
	slice.srcX = x;  slice.srcY = y;
	slice.srcW = w;  slice.srcH = h;

	// ��Ʈ�� ��ü
	slicedBitmap.Reset();
}

void TileMapRenderer::SetSkew(bool _setActive, FVector2 _skewAngle)
{
	if (_setActive) 
	{
		D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &m_effect);
		skewAngle = _skewAngle;
	}
	else 
	{
		m_effect.Reset();
		slicedBitmap.Reset();
		skewAngle = FVector2(0.0f, 0.0f);
	}
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> TileMapRenderer::GetSlicedBitmap(ID2D1Bitmap1* bitmap, const D2D1_RECT_F& srcRect)
{
	ComPtr<ID2D1Bitmap1> result;

	// 1. �߶� ���� ũ��
	D2D1_SIZE_U croppedSize = {
		static_cast<UINT32>(srcRect.right - srcRect.left),
		static_cast<UINT32>(srcRect.bottom - srcRect.top)
	};

	// 2. ���ο� ��Ʈ�� ����
	D2D1_BITMAP_PROPERTIES1 props = {
		{ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED },
		96.0f, 96.0f
	};
	D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateBitmap(
		croppedSize, nullptr, 0, &props, &result);

	// srcRect�� D2D1_RECT_U�� ��ȯ �ʿ�
	D2D1_RECT_U srcRectU = {
		static_cast<UINT32>(srcRect.left),
		static_cast<UINT32>(srcRect.top),
		static_cast<UINT32>(srcRect.right),
		static_cast<UINT32>(srcRect.bottom)
	};

	result->CopyFromBitmap(nullptr, bitmap, &srcRectU);

	return result;
}

// �� ��Ȯ�� Transform �ܰ躰 ����
D2D1_MATRIX_3X2_F TileMapRenderer::CreateTileTransform(float x, float y, float skewX, float skewY, const D2D1_MATRIX_3X2_F& viewMatrix)
{
	// 1. �⺻ ��ġ
	float baseX = x * tileWidth;
	float baseY = y * tileHeight;

	// 2. ���ͺ� ���� ������
	float depthOffset = y * tileHeight * std::tan(skewX * (Define::PI / 180.0f)) * 0.5f;
	float finalX = baseX + depthOffset;
	float finalY = baseY;

	// 3. Ÿ�� �߽���
	float centerX = finalX + tileWidth * 0.5f;
	float centerY = finalY + tileHeight * 0.5f;

	// 4. Transform ����
	D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Identity();

	// Skew�� ������ ����
	if (skewX != 0.0f || skewY != 0.0f)
	{
		transform = D2D1::Matrix3x2F::Translation(-centerX, -centerY) *  // �߽����� �̵�
			D2D1::Matrix3x2F::Skew(skewX, skewY) *               // Skew ����
			D2D1::Matrix3x2F::Translation(centerX, centerY);     // ���� ��ġ��
	}

	// 5. ī�޶�/���� Transform ����
	return viewMatrix * transform;
}