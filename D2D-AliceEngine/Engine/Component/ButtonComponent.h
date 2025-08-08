#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Define/Define.h>
#include <unordered_map>
#include <functional>
#include <Manager/TimerManager.h>

class Transform;
class ButtonComponent : public RenderComponent
{
public:
	ButtonComponent();
	~ButtonComponent();

	virtual void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;
	virtual FVector2 GetRelativeSize() override;

	void LoadData(Define::EButtonState state, const std::wstring& path);

	void ExecuteStateAction(Define::EButtonState state);
	void SetCurrentState(Define::EButtonState state);

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	std::unordered_map<Define::EButtonState, std::shared_ptr<ID2D1Bitmap1>> m_bitmaps;

	// 상태별 함수 설정 (안전한 버전)
	template<typename F>
	void SetStateAction(Define::EButtonState state, F&& action)
	{
		stateActionSlots[state] = { owner, action };
	}

	void SetActive(bool _active) { bActive = _active; }
	bool GetActive() const { return bActive; }
	
private:
	// UI 영역 내 마우스 위치 확인 헬퍼 메서드
	bool IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize);
	
	struct ClickFunctionSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	Define::EButtonState m_state = Define::EButtonState::Idle;
	bool m_prevMouseDown = false;
	
	// 상태별 함수 저장을 위한 unordered_map (안전한 버전)
	std::unordered_map<Define::EButtonState, ClickFunctionSlot> stateActionSlots;

	bool bActive = true;

private:
	// HoverLeave 용
	bool m_prevInArea = false;

	// 연출
private:
	FTimerHandle m_hoverTimer;
	float m_hoverT{ 0.f };
	float m_hoverAmp{ 0.08f };
	float m_hoverPeriod{ 0.8f };
	float m_hoverSharpness{ 1.2f }; // ← 모양 조절
	float m_hoverEase{ 0.7f };      // ← 모양 조절
	FVector2 m_hoverBaseScale{ 1.f, 1.f };

public:
	// 필요시 파라미터로 조절 가능
	void StartHoverPulse(float period = 0.8f, float amp = 0.08f);
	void StopHoverPulse();
};
