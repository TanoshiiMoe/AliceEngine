#include "pch.h"
#include "TileMapRenderer.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Helpers/FileHelper.h>

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
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
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
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());

	spriteInfo.height = GetBitmapSizeY();
	spriteInfo.width = GetBitmapSizeX();
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
}

void TileMapRenderer::Release()
{
	__super::Release();
}

void TileMapRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();

	// 잘라올 영역 결정 ― 값이 -1이면 원본 전부
	float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	float srcL = slice.srcX;
	float srcT = slice.srcY;

	D2D1_RECT_F srcRect = { srcL,               srcT,
							 srcL + cropW,       srcT + cropH };

	// 화면에 그릴 위치(센터링 포함)
	float offsetX = -cropW * spriteInfo.pivotX;
	float offsetY = -cropH * spriteInfo.pivotY;

	D2D1_RECT_F destRect = { (spriteInfo.x + offsetX),
							 (spriteInfo.y + offsetY),
							 (spriteInfo.x + offsetX + cropW),
							 (spriteInfo.y + offsetY + cropH) };


	if (!m_effect) {
		//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
		D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &srcRect);
	}
	else {
		// Skew Effect만 쓰는걸로 가정함.
		ComPtr<ID2D1Bitmap1> bitmap = GetSlicedBitmap(m_bitmap.get(), srcRect);
		m_effect->SetInput(0, bitmap.Get());

		D2D1_POINT_2F center = D2D1::Point2F(spriteInfo.x, spriteInfo.y);
		D2D1_MATRIX_3X2_F skewMat = D2D1::Matrix3x2F::Skew(skewAngle.x, skewAngle.y, center);
		HRESULT hr = m_effect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, skewMat);

		D2D1_POINT_2F destPos = D2D1::Point2F(offsetX, offsetY);
		D2DRenderManager::GetD2DDevice()->DrawImage(m_effect.Get(), &destPos);
	}
}

float TileMapRenderer::GetBitmapSizeX()
{
	return 0.0f;
}

float TileMapRenderer::GetBitmapSizeY()
{
	return 0.0f;
}

void TileMapRenderer::SetSlice(float x, float y, float w, float h)
{
	slice.srcX = x;  slice.srcY = y;
	slice.srcW = w;  slice.srcH = h;
}

void TileMapRenderer::SetSkew(bool _setActive, FVector2 _skewAngle)
{
	if (_setActive) {
		D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &m_effect);
		skewAngle = _skewAngle;
	}
	else {
		m_effect.Reset();
	}
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> TileMapRenderer::GetSlicedBitmap(ID2D1Bitmap1* bitmap, const D2D1_RECT_F& srcRect)
{
	ComPtr<ID2D1Bitmap1> result;

	// 1. 잘라낼 영역 크기
	D2D1_SIZE_U croppedSize = {
		static_cast<UINT32>(srcRect.right - srcRect.left),
		static_cast<UINT32>(srcRect.bottom - srcRect.top)
	};

	// 2. 새로운 비트맵 생성
	D2D1_BITMAP_PROPERTIES1 props = {
		{ DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED },
		96.0f, 96.0f
	};
	D2DRenderManager::GetInstance().m_d2dDeviceContext->CreateBitmap(
		croppedSize, nullptr, 0, &props, &result);

	// srcRect는 D2D1_RECT_U로 변환 필요
	D2D1_RECT_U srcRectU = {
		static_cast<UINT32>(srcRect.left),
		static_cast<UINT32>(srcRect.top),
		static_cast<UINT32>(srcRect.right),
		static_cast<UINT32>(srcRect.bottom)
	};

	result->CopyFromBitmap(nullptr, bitmap, &srcRectU);

	return result;
}

