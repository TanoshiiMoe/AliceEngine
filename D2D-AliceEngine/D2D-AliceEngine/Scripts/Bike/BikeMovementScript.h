#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class BikeMovementScript : public ScriptComponent
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

	// 속도 효과 적용 함수
	void ApplySlow(float slowFactor, float duration);
	void ApplyBoost(float boostFactor, float duration);

	// 현재 속도 출력
	float GetCurrSpeed() { return m_currentSpeed; }

	// 현재 속도변화 출력
	float GetSpeedModifier() { return m_speedModifier; }

	void Input();

	// Owner
	gameObject* GetOwnerObject() const { return m_owner; }
	void SetOwnerObject(gameObject* owner) { m_owner = owner; }

	// Initial Speed
	float GetInitialSpeed() const { return m_initialSpeed; }
	void SetInitialSpeed(float speed) { m_initialSpeed = speed; }

	// Max Speed
	float GetMaxSpeed() const { return m_maxSpeed; }
	void SetMaxSpeed(float speed) { m_maxSpeed = speed; }

	// Acceleration
	float GetAcceleration() const { return m_acceleration; }
	void SetAcceleration(float acceleration) { m_acceleration = acceleration; }

	// Current Speed
	float GetCurrentSpeed() const { return m_currentSpeed; }
	void SetCurrentSpeed(float speed) { m_currentSpeed = speed; }

	// Hit Reaction
	bool IsHitReaction() const { return m_hitReaction; }
	void SetHitReaction(bool hit) { m_hitReaction = hit; }

	// Hit Reaction Time
	float GetHitReactionTime() const { return m_hitReactionTime; }
	void SetHitReactionTime(float time) { m_hitReactionTime = time; }

	// Hit Timer
	float GetHitTimer() const { return m_hitTimer; }
	void SetHitTimer(float time) { m_hitTimer = time; }

	// Speed Modifier
	float GetSpeedModifierValue() const { return m_speedModifier; }
	void SetSpeedModifierValue(float modifier) { m_speedModifier = modifier; }

	// Modifier Duration
	float GetModifierDuration() const { return m_modifierDuration; }
	void SetModifierDuration(float duration) { m_modifierDuration = duration; }

	// m_prevMoveMount
	float GetPrevMoveAmount() const { return m_prevMoveAmount; }
	void SetPrevMoveAmount(float amount) { m_prevMoveAmount = amount; }

private:
	gameObject* m_owner = nullptr;

	// 기본 속도 설정
	float m_initialSpeed = 50.0f;
	float m_maxSpeed = 200.0f;
	float m_acceleration = 20.0f;

	// 현재 속도
	float m_currentSpeed = 0.0f;

	// 충돌 반응
	bool m_hitReaction = false;
	float m_hitReactionTime = 0.2f;
	float m_hitTimer = 0.0f;

	// 속도 변화 효과
	float m_speedModifier = 1.0f;
	float m_modifierDuration = 0.0f;

	float m_prevMoveAmount = 0.0f;
};

