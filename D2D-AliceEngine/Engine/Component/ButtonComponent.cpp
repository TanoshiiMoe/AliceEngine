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
#include <Manager/TimerManager.h>

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

	const bool mouseDown = Input::IsMouseLeftDown();
	const bool mouseUp = Input::IsMouseLeftReleased();

	const FVector2 mousePos = Input::GetMouseWorldPositionInCanvas();
	const FVector2 relativePos(
		relativeTransform.GetPosition().x,
		relativeTransform.GetPosition().y
	);

	const FVector2 uiSize = GetRelativeSize();
	const FVector2 finalUIPos = relativePos + FVector2(-uiSize.x * GetOwnerPivot()->x,
		-uiSize.y * GetOwnerPivot()->y);

	const bool inArea = IsMouseInUIArea(mousePos, finalUIPos, uiSize);

	if (inArea)
	{
		// (선택) 진입 이벤트가 필요하면 여기서 HoverEnter 발행:
		// if (!m_prevInArea) SetCurrentState(Define::EButtonState::HoverEnter);

		SetCursor(LoadCursorW(nullptr, IDC_HAND));

		if (mouseUp && m_prevMouseDown)
		{
			SetCurrentState(Define::EButtonState::Release);
		}
		else if (mouseDown)
		{
			SetCurrentState(Define::EButtonState::Pressed);
		}
		else
		{
			SetCurrentState(Define::EButtonState::Hover);
		}
	}
	else
	{
		// 직전 프레임엔 안에 있었고 이번 프레임에 나갔다면 HoverLeave 1회 발행
		if (m_prevInArea)
		{
			SetCurrentState(Define::EButtonState::HoverLeave);
		}
		else
		{
			SetCurrentState(Define::EButtonState::Idle);
		}

		SetCursor(LoadCursorW(nullptr, IDC_ARROW));
	}

	m_prevMouseDown = mouseDown;
	m_prevInArea = inArea;
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

void ButtonComponent::StartHoverPulse(float period, float amp)
{
	m_hoverPeriod = (period > 0.f) ? period : 0.8f;
	m_hoverAmp = amp;
	m_hoverT = 0.f;

	// 현재 스케일을 기준 스케일로 저장
	// (엔진 API에 맞춰서 GetScale/SetScale 또는 relativeTransform 사용)
	m_hoverBaseScale = relativeTransform.GetScale();

	auto weakThis = WeakFromThis<ButtonComponent>(); // 없다면 owner를 통해 안전검사
	TimerManager::GetInstance().SetTimerDt(m_hoverTimer, [weakThis](float dt) mutable
	{
		if (auto self = weakThis.lock())
		{
			self->m_hoverT += dt;

			// 0..1 구간 시간 좌표
			const float u = std::fmod(self->m_hoverT, self->m_hoverPeriod) / self->m_hoverPeriod;

			// [-1,1] 모양값
			const float wave = Math::eased_triangle_pulse(u, self->m_hoverSharpness, self->m_hoverEase);

			// 1 ± amp 로 스케일 변환
			const float s = 1.0f + self->m_hoverAmp * wave;

			const FVector2 newScale(self->m_hoverBaseScale.x * s,
				self->m_hoverBaseScale.y * s);

			self->relativeTransform.SetScale(newScale);
		}
	});
}

void ButtonComponent::StopHoverPulse()
{
	TimerManager::GetInstance().ClearTimer(m_hoverTimer);
	// 기준 스케일로 복원
	relativeTransform.SetScale(m_hoverBaseScale);
}
