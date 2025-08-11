#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>
#include "Drone.h"

enum class EBulletType
{
	Linear,
	BezierCurve,
	SinCurve
};

class gameObject;
class Bullet : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void UpdatePositionByType(const float& deltaSeconds);
	void UpdateCameraCulling();
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	gameObject* m_owner;

	EDroneType GetDroneType() const { return droneType; }
	void SetDroneType(EDroneType type) { droneType = type; }
	const std::wstring& GetSpritePath() const { return spritePath; }
	void SetSpritePath(const std::wstring& path) { spritePath = path; }
	
	// ������ ���� �Լ� �߰�
	float GetDamage() const { return damage; }
	void SetDamage(float val) { damage = val; }

public:
	FVector2 P0, P1, P2;
	float time = 0.0f;
	float duration = 1.0f; // �Ѿ� ���� �ð� (��)
	bool isActive = true;

	FVector2 inheritedVelocity; // �÷��̾� �ӵ� �����
	FVector2 accumulatedOffset; // �ӵ� ���� ��ġ
	bool bBezierFinished = false;

	FVector2 moveDir; // �Ѿ��� ���� ���� (Normalized)
	FVector2 startPos;
	FVector2 currentPos;
	float moveSpeed = 8.1f; // �Ѿ� �ӵ�

	float waveAmplitude = 0.2f; // ��鸲 ũ��
	float waveFrequency = 2.0f;  // ��鸲 �ӵ�

	EBulletType bulletType = EBulletType::BezierCurve;
	EDroneType droneType = EDroneType::Player; // �Ѿ� Ÿ��
	std::wstring spritePath = L"wallet.png";
	float damage = 5.0f; // ������ �� �߰�

	FTimerHandle handle;
	bool bCameraCulling = false;
};

