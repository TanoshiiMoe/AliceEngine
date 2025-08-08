#pragma once
#include "Core/Singleton.h"
#include <Manager/TimerManager.h>
#include <string>
#include <functional>
#include <vector>

enum class EGamePlayState : uint8_t {
    Idle,
    Running,
    Transitioning
};

class GamePlayManager : public Singleton<GamePlayManager>
{
public:
    void Initialize();
    void OnDestroy();

    // 상태 조회
    EGamePlayState GetState() const { return m_State; }
    const std::wstring& GetCurrentScene() const { return m_CurrentScene; }
    bool IsBusy() const { return m_State == EGamePlayState::Transitioning; }

    /**
     * @brief 기본 전환 (진행률 콜백 포함)
     * @param nextScene 전환할 씬 이름
     * @param durationSeconds 전환 시간(초)
     * @param splitPoint 0~1 구간 중 씬 교체 시점 (기본 0.5f)
     * @param onTick 진행률 콜백(t:0~1)
     */
    void TransitionToScene(const std::wstring& nextScene,
        float durationSeconds,
        float splitPoint = 0.5f,
        std::function<void(float)> onTick = nullptr);

    /**
     * @brief 페이드 연출이 자동으로 들어가는 간편 전환
     * SetFadeSetter()로 페이드 적용 함수를 먼저 지정해 두면, onTick 없이도 동작합니다.
     * @param nextScene 전환할 씬 이름
     * @param durationSeconds 전환 시간(초)
     * @param splitPoint 0~1 구간 중 씬 교체 시점 (기본 0.5f)
     */
    void TransitionWithFade(const std::wstring& nextScene,
        float durationSeconds,
        float splitPoint = 0.5f);

    /**
     * @brief 외부의 페이드 적용 함수를 등록 (0~1 알파)
     * 예: UIManager::SetFadeAlpha(float)
     */
    void SetFadeSetter(std::function<void(float)> setter) { m_FadeSetter = std::move(setter); }

    /**
     * @brief 간단 지연 실행 (버튼 연출용)
     * @param seconds 지연(초)
     * @param cb 지연 후 실행할 콜백
     * @param outHandle (옵션) 핸들이 필요하면 포인터로 받기
     */
    void RunAfter(float seconds, std::function<void()> cb, FTimerHandle* outHandle = nullptr);

private:
    void SafeChangeScene();
    void EndTransition();
    void CancelTransition();

    // 기본 이징 (스무스)
    static float EaseInOutQuad(float x);

private:
    EGamePlayState m_State = EGamePlayState::Idle;
    std::wstring   m_CurrentScene;
    std::wstring   m_PendingScene;

    bool   m_HasActiveTransition = false;
    bool   m_SwitchedAtSplit = false;
    float  m_Duration = 0.0f;
    float  m_SplitPoint = 0.5f;

    FTimerHandle m_TransitionTimer;

    // 선택: 자동 페이드용
    std::function<void(float)> m_FadeSetter = nullptr;

    // 임시 지연용 핸들들(매니저 소멸 시 정리)
    std::vector<FTimerHandle> m_TempDelayHandles;
};
