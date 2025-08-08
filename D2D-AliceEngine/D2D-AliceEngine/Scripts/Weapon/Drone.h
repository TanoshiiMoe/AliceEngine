#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>

class gameObject;
class SpriteRenderer;
class Drone : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnCollisionEnter2D(Collision2D* collider) override;
	virtual void OnCollisionStay2D(Collision2D* collider) override;
	virtual void OnCollisionExit2D(Collision2D* collider) override;
	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	gameObject* m_player = nullptr;
	gameObject* m_owner = nullptr;

	SpriteRenderer* body = nullptr;
	SpriteRenderer* arm = nullptr;

	FVector2 initBodyPos = FVector2(-50.0f, 40.0f);

	FTimerHandle timer;
	bool bCanFire = true;

	// 위아래 진폭과 속도
	float elapsed = 0.0f;
	float duration = 1.0f; // 위→아래 한 번 이동하는데 걸리는 시간
	float startY = 0.0f;
	float endY = 7.0f;    // 위로 5만큼 이동
	bool goingUp = true;
};
