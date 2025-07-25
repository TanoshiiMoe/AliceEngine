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
	PackageResourceManager::GetInstance().UnloadData(filePath); // ��Ʈ�� ��ε�
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
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
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

	// TransformComponent���� ��ġ�� �޾ƿ� (ũ��, ȸ�� ���� �ʿ�� �߰�)
	TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
	if (!transform) return;

	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Scale(transform->GetScale().x, transform->GetScale().y) *
		D2D1::Matrix3x2F::Rotation(transform->GetRotation() * 180.0f / Define::PI) *
		D2D1::Matrix3x2F::Translation(transform->GetPosition().x, transform->GetPosition().y);

	// ī�޶� �����ϰ� ��ġ ��� UI �׸���
	D2DRenderManager::GetD2DDevice()->SetTransform(mat);

	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}
