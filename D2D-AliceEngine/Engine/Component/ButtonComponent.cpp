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
				FVector2 mousePos = Input::GetMousePosition();
				FVector2 ownerPos = FVector2(relativeTransform.GetPosition().x, relativeTransform.GetPosition().y);
				float width = GetScale().x;
				float height = GetScale().y;
				if (mousePos.x >= ownerPos.x &&
					mousePos.x <= ownerPos.x + width &&
					mousePos.y >= ownerPos.y &&
					mousePos.y <= ownerPos.y + height)
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
	if (m_bitmap == nullptr) return;
	__super::Render();

	// 카메라 무시하고 위치 기반 UI 그리기
	D2DRenderManager::GetD2DDevice()->SetTransform(view);
	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get());
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

void ButtonComponent::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

FVector2 ButtonComponent::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}