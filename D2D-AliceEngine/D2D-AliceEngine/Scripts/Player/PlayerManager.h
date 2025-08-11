#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimatorInstance.h"
#include "Animation/AnimationController.h"
#include <Manager/TimerManager.h>

class PlayerManager : public  ScriptComponent
{
public:
	static PlayerManager* instance;

	void Initialize() override;
	void OnStart() override;
	void OnEnd() override;
	void Update(const float& deltaSeconds) override;

	// 플레이어를 잔상과 함께 서서히 사라지도록 파괴
	void DelayDestroy();

	// 플레이어 타임스케일 획득
	float GetTimeScale() { return playerTimeScale; }

private:
	AnimatorInstance* animInstance = nullptr;
	AnimatorController animController;

	void Input();

	float playerTimeScale = 1.0f;

	// ===== DelayDestroy =====
	bool m_isDelaying = false;
	bool m_skipGhostOnDestroy = false;
	bool m_isFading = false;
	float m_fadeDuration = 1.0f;
	float m_fadeElapsed = 0.0f;
	class SpriteRenderer* m_fadeTargetSR = nullptr;
	class gameObject* m_ghostGO = nullptr;
	FTimerHandle m_fadeHandle;
};

