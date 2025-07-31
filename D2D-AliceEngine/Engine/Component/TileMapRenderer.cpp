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

			float quarterViewOffset = y * tileHeight * std::tan(skewAngle.x * (Define::PI / 180.0f)) * 0.5f;

			// destRect�� ���ͺ� ������ ����
			D2D1_RECT_F destRect = D2D1::RectF(
				(FLOAT)(x * tileWidth) + quarterViewOffset,     // X�� ������ �߰�
				(FLOAT)(y * tileHeight),                        // Y�� �״��
				(FLOAT)((x + 1) * tileWidth) + quarterViewOffset, // �����ʵ� ���� ������
				(FLOAT)((y + 1) * tileHeight)
			);
			D2D1_RECT_U srcRect = { (UINT32)sx, (UINT32)sy, (UINT32)(sx + tileWidth), (UINT32)(sy + tileHeight) };

			D2D1_POINT_2F center = D2D1::Point2F(
				(destRect.left + destRect.right) * 0.5f,   // Ÿ�� �߽� X
				(destRect.top + destRect.bottom) * 0.5f    // Ÿ�� �߽� Y
			);
			D2D1_MATRIX_3X2_F skewMat = D2D1::Matrix3x2F::Skew(skewAngle.x, skewAngle.y, center);
			//D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Skew(20);
			//D2D1_MATRIX_3X2_F skewMat = D2D1::Matrix3x2F::Skew(30, 0, center);
			 
			//D2D1_RECT_F srcRect = { (UINT32)sx, (UINT32)sy, (UINT32)(sx + tileWidth), (UINT32)(sy + tileHeight) };
			//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);
			HRESULT hr = D2DRenderManager::GetInstance().m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, &skewMat);
			//HRESULT hr = D2DRenderManager::GetInstance().m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, nullptr);

			//���� ��ǥ�� �߰��ϸ� 3D ��ǥ�迡���� ��ġ�� �ǹǷ�, 3D ȿ���� ����.
			//D2D1_MATRIX_3X2_F finalTransform = view * skewMat;
			//HRESULT hr = D2DRenderManager::GetInstance().m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, &finalTransform);
			assert(SUCCEEDED(hr));
		}
	}
	int batchSize = batch->GetSpriteCount();
	assert(batchSize > 0);
	context->DrawSpriteBatch(
		batch,
		0, batchSize,
		m_bitmap.get(),
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1_SPRITE_OPTIONS_NONE);

	//if (!m_effect) {
	//	// �߶�� ���� ���� �� ���� -1�̸� ���� ����
	//	float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	//	float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	//	float srcL = slice.srcX;
	//	float srcT = slice.srcY;

	//	D2D1_RECT_F srcRect = { srcL,               srcT,
	//							 srcL + cropW,       srcT + cropH };

	//	// ȭ�鿡 �׸� ��ġ(���͸� ����)
	//	float offsetX = -cropW * spriteInfo.pivotX;
	//	float offsetY = -cropH * spriteInfo.pivotY;

	//	D2D1_RECT_F destRect = { (spriteInfo.x + offsetX),
	//							 (spriteInfo.y + offsetY),
	//							 (spriteInfo.x + offsetX + cropW),
	//							 (spriteInfo.y + offsetY + cropH) };

	//	//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
	//	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);
	//}
	//else {
	//	// Skew Effect�� ���°ɷ� ������.
	//	if (!slicedBitmap) {
	//		// �߶�� ���� ���� �� ���� -1�̸� ���� ����
	//		float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	//		float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	//		float srcL = slice.srcX;
	//		float srcT = slice.srcY;

	//		D2D1_RECT_F srcRect = { srcL,               srcT,
	//								 srcL + cropW,       srcT + cropH };

	//		slicedBitmap = GetSlicedBitmap(m_bitmap.get(), srcRect);
	//	}

	//	m_effect->SetInput(0, slicedBitmap.Get());

	//	// ������
	//	float offsetX = -static_cast<float>(slicedBitmap->GetPixelSize().width) * spriteInfo.pivotX;
	//	float offsetY = -static_cast<float>(slicedBitmap->GetPixelSize().height) * spriteInfo.pivotY;

	//	D2D1_POINT_2F center = D2D1::Point2F(-offsetX, -offsetY);
	//	D2D1_MATRIX_3X2_F skewMat = D2D1::Matrix3x2F::Skew(skewAngle.x, skewAngle.y, center);
	//	m_effect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, skewMat);

	//	D2D1_POINT_2F destPos = D2D1::Point2F(offsetX, offsetY);
	//	D2DRenderManager::GetD2DDevice()->DrawImage(m_effect.Get());// , & destPos);
	//}
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