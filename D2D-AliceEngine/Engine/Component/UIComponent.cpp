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

	// TransformComponent에서 위치만 받아옴 (크기, 회전 포함 필요시 추가)
	TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
	if (!transform) return;
	
	// 좌표계는 유니티 좌표계가 아닌 D2D기반, 뒤 숫자는 크기 (이거 래핑해서 생성할때 크기 정해두고 띄우면 될듯)
	// 제가 만든 버튼도 이런식이었던거긴해요
	D2D1_RECT_F destRect = { 0, 0, 100, 100};

	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Scale(transform->GetScale().x, transform->GetScale().y) *
		D2D1::Matrix3x2F::Rotation(transform->GetRotation() * 180.0f / Define::PI) *
		D2D1::Matrix3x2F::Translation(transform->GetPosition().x, transform->GetPosition().y);

	// 카메라 무시하고 위치 기반 UI 그리기
	D2DRenderManager::GetD2DDevice()->SetTransform(mat);

	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

float UIComponent::GetSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float UIComponent::GetSizeY()
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
