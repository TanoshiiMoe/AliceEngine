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
#include <Helpers/CoordHelper.h>

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

	if (!bActive) return;

	bool mouseDown = Input::IsMouseLeftDown();
	bool mouseUp = Input::IsMouseLeftReleased();
	FVector2 mousePos = Input::GetMouseWorldPositionInCanvas();
	FVector2 relativePos = FVector2(
		relativeTransform.GetPosition().x,
		relativeTransform.GetPosition().y
	);

	FVector2 uiSize = GetRelativeSize();
	FVector2 finalUIPos = relativePos + FVector2(-uiSize.x * GetOwnerPivot()->x, -uiSize.y * GetOwnerPivot()->y);

	// UI 영역 내 마우스 클릭 확인
	if (IsMouseInUIArea(mousePos, finalUIPos, uiSize))
	{
		// 마우스가 UI 영역 안에 있을 때만 함수 호출
		SetCursor(LoadCursorW(nullptr, IDC_HAND));

		if (mouseUp && m_prevMouseDown) // 마우스가 떼어졌고 이전에 눌려있었다면 Release 상태
		{
			SetCurrentState(EButtonState::Release);
		}
		else if (mouseDown) // 마우스가 눌려있으면 Pressed 상태
		{
			SetCurrentState(EButtonState::Pressed);
		}
		else
		{
			SetCurrentState(EButtonState::Hover); // 마우스가 올라가있지만 눌리지 않았으면 Hover 상태
		}
	}
	else
	{
		SetCurrentState(EButtonState::Idle);
		SetCursor(LoadCursorW(nullptr, IDC_ARROW));
	}

	m_prevMouseDown = mouseDown;
}

void ButtonComponent::Release()
{
	__super::Release();
	stateActionSlots.clear();
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

void ButtonComponent::LoadData(Define::EButtonState state, const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmaps[state] = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
	
	// 현재 상태의 이미지라면 m_bitmap도 업데이트
	if (state == m_state)
	{
		m_bitmap = m_bitmaps[state];
	}
}

bool ButtonComponent::IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize)
{
	return (mousePos.x >= uiPos.x &&
			mousePos.x <= uiPos.x + uiSize.x &&
			mousePos.y >= uiPos.y &&
			mousePos.y <= uiPos.y + uiSize.y);
}

void ButtonComponent::ExecuteStateAction(Define::EButtonState state)
{
	auto it = stateActionSlots.find(state);
	if (it != stateActionSlots.end())
	{
		// 부모 객체가 유효한지 확인
		if (!it->second.weakPtr.expired())
		{
			it->second.func(); // 해당 상태의 함수 실행
		}
		else
		{
			// 부모 객체가 파괴되었으면 슬롯 제거
			stateActionSlots.erase(it);
		}
	}
}

void ButtonComponent::SetCurrentState(Define::EButtonState state)
{
	// 이전 상태와 다르면 상태 변경
	if (m_state != state)
	{
		// 이전 상태의 종료 함수 실행 (있는 경우)
		ExecuteStateAction(static_cast<Define::EButtonState>(static_cast<int>(m_state) + 100)); // 종료 상태는 +100으로 가정
		
		m_state = state;
		
		// 새 상태의 시작 함수 실행
		ExecuteStateAction(state);
		
		// 현재 상태에 맞는 이미지로 m_bitmap 업데이트
		auto it = m_bitmaps.find(state);
		if (it != m_bitmaps.end() && it->second)
		{
			m_bitmap = it->second;
		}
	}
}