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

    // �Ѿ� ������ ��� �� ���� �ı�
    void DelayDestroy();
protected:
	AnimatorController animController;

	FTimerHandle timer;
    // ���� �ı� ����
    bool m_isDelaying = false;
    bool m_skipGhostOnDestroy = false;
    FTimerHandle m_fadeHandle;

    // ���̵� ����
    bool m_isFading = false;
    float m_fadeDuration = 1.0f;
    float m_fadeElapsed = 0.0f;
    class SpriteRenderer* m_fadeTargetSR = nullptr; // ��Ʈ �Ǵ� ���� ���
    class gameObject* m_ghostGO = nullptr;          // ������ ��Ʈ ������Ʈ
};

