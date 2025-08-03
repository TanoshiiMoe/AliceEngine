#include "pch.h"
#include "ButtonComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Manager/UpdateTaskManager.h>
#include <Core/Input.h>
#include <Helpers/FileHelper.h>

ButtonComponent::ButtonComponent()
{
	drawType = Define::EDrawType::ScreenSpace;
}

ButtonComponent::~ButtonComponent()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void ButtonComponent::Initialize()
{
	__super::Initialize();

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostUpdateWork);
}

void ButtonComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	for (auto it = slots.begin(); it != slots.end();)
	{
		if (!it->weakPtr.expired())
		{
			if (Input::IsMouseLeftPressed())
			{
				// 마우스 위치 가져오기
				FVector2 mousePos = Input::GetMousePosition();
				
				// 컴포넌트의 상대좌표
				FVector2 relativePos = FVector2(
					relativeTransform.GetPosition().x, 
					relativeTransform.GetPosition().y
				);
				
				// UI 크기 가져오기 및 위치 계산 (부모 좌표 + 상대좌표)
				FVector2 uiSize = GetRelativeSize();
				FVector2 finalUIPos = relativePos + FVector2(-uiSize.x * GetOwnerPivot()->x, -uiSize.y * GetOwnerPivot()->y);

				// UI 영역 내 마우스 클릭 확인
				if (IsMouseInUIArea(mousePos, finalUIPos, uiSize))
				{
					// 마우스가 UI 영역 안에 있을 때만 함수 호출
					it->func();
				}
			}
			it++;
		}
		else
		{
			it = slots.erase(it);
		}
	}
}

void ButtonComponent::Release()
{
	__super::Release();
}

void ButtonComponent::Render()
{
	if (!m_bitmap) return;
	__super::Render();

	FVector2 relativeSize = FVector2(GetBitmapSizeX(), GetBitmapSizeY());
	D2D1_RECT_F destRect = D2D1::RectF(-relativeSize.x / 2, -relativeSize.y / 2, relativeSize.x / 2, relativeSize.y / 2);
	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), destRect);
}

float ButtonComponent::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float ButtonComponent::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 ButtonComponent::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void ButtonComponent::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

bool ButtonComponent::IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize)
{
	return (mousePos.x >= uiPos.x &&
			mousePos.x <= uiPos.x + uiSize.x &&
			mousePos.y >= uiPos.y &&
			mousePos.y <= uiPos.y + uiSize.y);
}