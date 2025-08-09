#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimationController.h"
#include <Manager/TimerManager.h>
class Car : public ScriptComponent
{
public:
	Car() {}
	virtual ~Car() {}

	void Initialize() override;
	void OnStart() override;
    void OnDestroy() override;
    void Update(const float& deltaSeconds) override;

    // 총알 등으로 사망 시 지연 파괴
    void DelayDestroy();
protected:
	AnimatorController animController;

	FTimerHandle timer;
    // 지연 파괴 상태
    bool m_isDelaying = false;
    bool m_skipGhostOnDestroy = false;
    FTimerHandle m_fadeHandle;

    // 페이드 제어
    bool m_isFading = false;
    float m_fadeDuration = 1.0f;
    float m_fadeElapsed = 0.0f;
    class SpriteRenderer* m_fadeTargetSR = nullptr; // 고스트 또는 폴백 대상
    class gameObject* m_ghostGO = nullptr;          // 생성된 고스트 오브젝트
};

