#include "pch.h"
#include "UIImage.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>

UIImage::UIImage()
{
}

UIImage::~UIImage()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void UIImage::Initialize()
{
	__super::Initialize();
}

void UIImage::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void UIImage::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

void UIImage::Release()
{
	__super::Release();
}

void UIImage::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();

	D2D1_POINT_2F pivot =
	{
		GetSizeX() * GetPivot()->x,
		GetSizeY() * GetPivot()->y
	};
	D2D1_RECT_F destRect = { -pivot.x, -pivot.y,  pivot.x,  pivot.y };

	// TransformComponent에서 위치만 받아옴 (크기, 회전 포함 필요시 추가)
	TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
	if (!transform) return;

	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Scale(transform->GetScale().x, transform->GetScale().y) *
		D2D1::Matrix3x2F::Rotation(transform->GetRotation() * 180.0f / Define::PI) *
		D2D1::Matrix3x2F::Translation(transform->GetPosition().x, transform->GetPosition().y);

	// 카메라 무시하고 위치 기반 UI 그리기
	D2DRenderManager::GetD2DDevice()->SetTransform(mat);

	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}
