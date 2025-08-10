#pragma once
#include "Core/Singleton.h"
#include <Manager/TimerManager.h>
#include <string>
#include <functional>

// 게임 상태(전역)
enum class EGameRunState : uint8_t {
    Boot,        // 초기 부팅/런처 직후
    Loading,     // 리소스/씬 로딩 중
    InGame,      // 실제 플레이 중
    Paused,      // 일시정지
    GameOver,    // 패배
    Victory,     // 승리
    Transitioning// 씬 전환 중(외부(UI)가 처리)
};

class GamePlayManager : public Singleton<GamePlayManager>
{
public:
    GamePlayManager();
    ~GamePlayManager();

    // ===== 조회 =====
    EGameRunState GetState() const { return m_State; }
    bool IsPlaying()        const { return m_State == EGameRunState::InGame; }
    bool IsPaused()         const { return m_State == EGameRunState::Paused; }
    bool IsBusy()           const { return m_State == EGameRunState::Transitioning || m_State == EGameRunState::Loading; }

    const std::wstring& GetCurrentScene() const { return m_CurrentScene; }
    const std::wstring& GetPendingScene() const { return m_PendingScene; }

    // ===== 상태 변경(외부(UI)에서 호출) =====
    // 플레이 시작: InGame 진입
    void StartGame();

    void GameOver();
    void GameClear();

    void SpawnVignette(float durationSec, float maxAlpha);

    void SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha);

    // 일시정지/재개 (글로벌 타임스케일 제어만 수행)
    void PauseGame();
    void ResumeGame();
    void TogglePause();

    // 종료(게임오버/승리)
    void EndGameAsGameOver();
    void EndGameAsVictory();

    // ===== 씬 전환: 타이머/연출 없음. UI가 연출/타이머 처리하고, 여기엔 마킹만 =====
    // 전환 시작: 상태를 Transitioning으로 두고 PendingScene 기록
    void MarkTransitionBegin(const std::wstring& nextScene);
    // 전환 실제 완료 시(UI에서 씬 교체 후) 호출: 현재 씬 갱신 + 상태 복귀
    void MarkTransitionComplete();

    // 로딩 구간(선택): UI가 로딩 시작/완료를 알릴 수 있음
    void MarkLoadingBegin();
    void MarkLoadingComplete();

    // 상태 변경 콜백(선택): (prev, now)
    void SetOnStateChanged(std::function<void(EGameRunState, EGameRunState)> cb) { m_OnStateChanged = std::move(cb); }

    // 전역 타임스케일 Get/Set (필요하면 사용)
    float GetSavedTimeScale() const { return m_PrevTimeScale; }

private:
    void SetState(EGameRunState newState);
    void ApplyPauseTimescale(bool paused); // TimerManager의 GlobalTimeScale만 제어

private:
    EGameRunState m_State = EGameRunState::Boot;

    std::wstring  m_CurrentScene; // 현재 활성 씬명(외부가 바꾼 뒤 MarkTransitionComplete에서 세팅)
    std::wstring  m_PendingScene; // 전환 예정 씬명(외부가 MarkTransitionBegin로 세팅)

    // 일시정지/재개용 이전 타임스케일 저장
    float m_PrevTimeScale = 1.0f;

    // 상태 변경 이벤트(옵션)
    std::function<void(EGameRunState, EGameRunState)> m_OnStateChanged = nullptr;

    FTimerHandle gameOverTimer;
    FTimerHandle gameOverTransitionTimer;
};
