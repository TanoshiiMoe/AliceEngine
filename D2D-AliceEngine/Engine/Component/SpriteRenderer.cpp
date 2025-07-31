#include "pch.h"
#include "SpriteRenderer.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>

SpriteRenderer::SpriteRenderer()
{
	spriteInfo.height = -1;
	spriteInfo.width = -1;
	spriteInfo.x = 0;
	spriteInfo.y = 0;
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
	spriteInfo.name = "";
}

SpriteRenderer::~SpriteRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void SpriteRenderer::Initialize()
{
	__super::Initialize();
}

void SpriteRenderer::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void SpriteRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());

	spriteInfo.height = GetBitmapSizeY();
	spriteInfo.width = GetBitmapSizeX();
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
}

void SpriteRenderer::Release()
{
	__super::Release();
}

/*
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
*/

void SpriteRenderer::Render()
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

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context) return;
	
	// 이펙트 있을시 이펙트 그리기
	if (!m_effect)
	{
		D2D1::Matrix3x2F backToD2DTransform = D2D1::Matrix3x2F::Translation(-GetBitmapSizeX() / 2, -GetBitmapSizeY() / 2);
		D2D1::Matrix3x2F pivotTransform = D2D1::Matrix3x2F::Translation((GetBitmapSizeX() / 2) * (GetPivot()->x - 0.5f), (GetBitmapSizeY() / 2) * (GetPivot()->y - 0.5f));
		context->SetTransform(backToD2DTransform * pivotTransform * view);
		D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get());
	}
	else {
		D2D1_POINT_2F destPos = D2D1::Point2F(offsetX, offsetY);
		D2DRenderManager::GetD2DDevice()->DrawImage(m_effect.Get(), &destPos, &srcRect);
	}
		
}

float SpriteRenderer::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float SpriteRenderer::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 SpriteRenderer::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}

void SpriteRenderer::SetSlice(float x, float y, float w, float h)
{
	slice.srcX = x;  slice.srcY = y;
	slice.srcW = w;  slice.srcH = h;
}

void SpriteRenderer::SetSkewing(bool _isSkewing, FVector2 _skewAngle /*= FVector2(0.0f, 0.0f)*/)
{
	
}
