#include "pch.h"
#include "UIImage.h"
#include <Helpers/FileHelper.h>
#include <Manager/PackageResourceManager.h>
#include <Component/TransformComponent.h>
#include <Component/Component.h>

#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Object/Camera.h>

#include <System/RenderSystem.h>
#include <Math/TColor.h>

UIImage::UIImage()
{
}

UIImage::~UIImage()
{

}

void UIImage::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

void UIImage::SetPosition(const FVector2& pos)
{
	m_transform.SetPosition(pos.x, pos.y);
}

void UIImage::SetPivot(float _x, float _y)
{
	owner->transform()->SetPivot(_x, _y);
}

void UIImage::SetAnchor(EUIScreenAnchor anchor, const FVector2& offset)
{
	m_anchor = anchor;

	FVector2 pos = FVector2(0, 0);
	SetPivot(0.5f, 0.5f);

	float screenWidth = SCREEN_WIDTH - GetScale().x;
	float screenHeight = SCREEN_HEIGHT - GetScale().y;

	switch (anchor)
	{
	case EUIScreenAnchor::TopLeft:
		break;

	case EUIScreenAnchor::TopCenter:
		pos = FVector2(screenWidth / 2.0f, 0);
		break;

	case EUIScreenAnchor::TopRight:
		pos = FVector2(screenWidth, 0);
		break;

	case EUIScreenAnchor::MiddleLeft:
		pos = FVector2(0, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::MiddleCenter:
		pos = FVector2(screenWidth / 2.0f, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::MiddleRight:
		pos = FVector2(screenWidth, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::BottomLeft:
		pos = FVector2(0, screenHeight);
		break;

	case EUIScreenAnchor::BottomCenter:
		pos = FVector2(screenWidth / 2.0f, screenHeight);
		break;

	case EUIScreenAnchor::BottomRight:
		pos = FVector2(screenWidth, screenHeight);
		break;

	default:
		break;
	}

	//Offset은 D2D좌표계를 사용합니다.
	SetPosition(pos + offset);
}

void UIImage::SetAnchor(EUIScreenAnchor anchor, const float& offsetX, const float& offsetY)
{
	m_anchor = anchor;

	FVector2 pos = FVector2(0, 0);
	SetPivot(0.5f, 0.5f);

	float screenWidth = SCREEN_WIDTH - GetScale().x;
	float screenHeight = SCREEN_HEIGHT - GetScale().y;

	switch (anchor)
	{
	case EUIScreenAnchor::TopLeft:
		break;

	case EUIScreenAnchor::TopCenter:
		pos = FVector2(screenWidth / 2.0f, 0);
		break;

	case EUIScreenAnchor::TopRight:
		pos = FVector2(screenWidth, 0);
		break;

	case EUIScreenAnchor::MiddleLeft:
		pos = FVector2(0, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::MiddleCenter:
		pos = FVector2(screenWidth / 2.0f, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::MiddleRight:
		pos = FVector2(screenWidth, screenHeight / 2.0f);
		break;

	case EUIScreenAnchor::BottomLeft:
		pos = FVector2(0, screenHeight);
		break;

	case EUIScreenAnchor::BottomCenter:
		pos = FVector2(screenWidth / 2.0f, screenHeight);
		break;

	case EUIScreenAnchor::BottomRight:
		pos = FVector2(screenWidth, screenHeight);
		break;

	default:
		break;
	}

	//Offset은 D2D좌표계를 사용합니다.
	SetPosition(pos + FVector2(offsetX,offsetY));
}
