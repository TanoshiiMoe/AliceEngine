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

	// �ӵ� ȿ�� ���� �Լ�
	void ApplySlow(float slowFactor, float duration);
	void ApplyBoost(float boostFactor, float duration);

	void Input();

private:
	gameObject* m_owner = nullptr;

	// �⺻ �ӵ� ����
	float m_initialSpeed = 50.0f;
	float m_maxSpeed = 300.0f;
	float m_acceleration = 20.0f;

	// ���� �ӵ�
	float m_currentSpeed = 0.0f;

	// �浹 ����
	bool m_hitReaction = false;
	float m_hitReactionTime = 0.2f;
	float m_hitTimer = 0.0f;

	// �ӵ� ��ȭ ȿ��
	float m_speedModifier = 1.0f;
	float m_modifierDuration = 0.0f;
};

