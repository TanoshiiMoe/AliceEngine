#include "pch.h"
#include "GamePlayManager.h"
#include <Manager/SceneManager.h>
#include <Helpers/Logger.h>

void GamePlayManager::Initialize()
{
    m_State = EGamePlayState::Idle;
    m_CurrentScene.clear();
    m_HasActiveTransition = false;
    m_Duration = 0.0f;
    m_SwitchedAtSplit = false;
    m_TempDelayHandles.clear();
}

void GamePlayManager::OnDestroy()
{
    CancelTransition();
    // 등록된 지연 타이머들 정리
    auto& TM = TimerManager::GetInstance();
    for (auto& h : m_TempDelayHandles)
        if (TM.IsTimerActive(h)) TM.ClearTimer(h);
    m_TempDelayHandles.clear();
}

void GamePlayManager::TransitionToScene(const std::wstring& nextScene,
    float durationSeconds,
    float splitPoint,
    std::function<void(float)> onTick)
{
    CancelTransition();

    m_State = EGamePlayState::Transitioning;
    m_HasActiveTransition = true;
    m_PendingScene = nextScene;
    m_Duration = (durationSeconds > 0.0f) ? durationSeconds : 0.0001f;
    m_SplitPoint = splitPoint;
    m_SwitchedAtSplit = false;

    auto& TM = TimerManager::GetInstance();
    float elapsed = 0.0f;

    TM.SetTimerDt(m_TransitionTimer, [this, &elapsed, onTick](float dt) mutable {
        if (!m_HasActiveTransition) return;

        elapsed += dt;
        float t = elapsed / m_Duration;
        if (t > 1.0f) t = 1.0f;

        // Split 지점에서 씬 교체
        if (!m_SwitchedAtSplit && m_SplitPoint > 0.0f && t >= m_SplitPoint) {
            SafeChangeScene();
        }

        // 진행률 콜백
        if (onTick) onTick(t);

        // 종료 처리
        if (elapsed >= m_Duration) {
            if (!m_SwitchedAtSplit) {
                SafeChangeScene();
            }
            EndTransition();
        }
    });
}

void GamePlayManager::TransitionWithFade(const std::wstring& nextScene,
    float durationSeconds,
    float splitPoint)
{
    // 페이드 지정이 없으면 기본 전환만 수행
    if (!m_FadeSetter) {
        TransitionToScene(nextScene, durationSeconds, splitPoint, nullptr);
        return;
    }

    // 페이드 자동 onTick
    auto fadeTick = [this, splitPoint](float t) {
        // 0~split : 페이드 아웃(0→1), split~1 : 페이드 인(1→0)
        float fadeAlpha = 0.0f;
        if (splitPoint <= 0.0f) {
            // 바로 씬 전환 상태: 전 구간을 페이드 인으로만
            fadeAlpha = 1.0f - EaseInOutQuad(t);
        }
        else if (splitPoint >= 1.0f) {
            // 전환 막판에 씬 체인지: 전 구간을 페이드 아웃만
            fadeAlpha = EaseInOutQuad(t);
        }
        else if (t <= splitPoint) {
            float tt = t / splitPoint; // 0~1
            fadeAlpha = EaseInOutQuad(tt); // 0→1
        }
        else {
            float tt = (t - splitPoint) / (1.0f - splitPoint); // 0~1
            fadeAlpha = 1.0f - EaseInOutQuad(tt); // 1→0
        }
        m_FadeSetter(fadeAlpha);
    };

    TransitionToScene(nextScene, durationSeconds, splitPoint, fadeTick);
}

void GamePlayManager::RunAfter(float seconds, std::function<void()> cb, FTimerHandle* outHandle)
{
    FTimerHandle h;
    TimerManager::GetInstance().SetTimer(h, std::move(cb), seconds, /*bLoop=*/false, /*FirstDelay=*/seconds);

    if (outHandle) {
        *outHandle = h;
    }
    else {
        m_TempDelayHandles.push_back(h);
    }
}

void GamePlayManager::SafeChangeScene()
{
    CancelTransition(); // 씬 전환 전에 타이머 해제
    SceneManager::GetInstance().ChangeScene(m_PendingScene);
    m_CurrentScene = m_PendingScene;
    m_PendingScene.clear();
    m_SwitchedAtSplit = true;

    LOG_INFO(L"[Transition] Scene changed to: %s", m_CurrentScene.c_str());
}

void GamePlayManager::EndTransition()
{
    CancelTransition();
    m_State = EGamePlayState::Running;
}

void GamePlayManager::CancelTransition()
{
    if (TimerManager::GetInstance().IsTimerActive(m_TransitionTimer))
        TimerManager::GetInstance().ClearTimer(m_TransitionTimer);

    m_HasActiveTransition = false;
}

float GamePlayManager::EaseInOutQuad(float x)
{
    // x: 0~1
    return (x < 0.5f) ? (2.0f * x * x) : (1.0f - (float)std::pow(-2.0f * x + 2.0f, 2.0f) * 0.5f);
}
