#include "pch.h"
#include "UIComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>

UIComponent::UIComponent()
{
	drawType = Define::EDrawType::ScreenSpace;
}

UIComponent::~UIComponent()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void UIComponent::Initialize()
{
	__super::Initialize();
}

void UIComponent::Update()
{
	__super::Update();
}

void UIComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void UIComponent::Release()
{
	__super::Release();
}

void UIComponent::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();
	
	D2D1_RECT_F destRect = { 0, 0, scale.x, scale.y};

	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Scale(m_transform.GetScale().x, m_transform.GetScale().y) *
		D2D1::Matrix3x2F::Rotation(m_transform.GetRotation() * 180.0f / Define::PI) *
		D2D1::Matrix3x2F::Translation(m_transform.GetPosition().x, m_transform.GetPosition().y);

	// 카메라 무시하고 위치 기반 UI 그리기
	D2DRenderManager::GetD2DDevice()->SetTransform(mat);

	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

float UIComponent::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float UIComponent::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 UIComponent::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}
