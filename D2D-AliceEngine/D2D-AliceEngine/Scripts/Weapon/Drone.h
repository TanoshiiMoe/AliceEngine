#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>

struct FDroneSpritePath
{
	std::wstring body;
	std::wstring arm;

	FDroneSpritePath() : body(L""), arm(L"") {}
	FDroneSpritePath(std::wstring _body, std::wstring _arm) : body(_body), arm(_arm) {}
	void operator= (const FDroneSpritePath& other)
	{
		body = other.body;
		arm = other.arm;
	}
};

enum class EDroneType
{
	Player,
	Enemy
};
class gameObject;
class SpriteRenderer;
class Drone : public ScriptComponent
{
public:
	Drone(FDroneSpritePath path);
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void Floating(const float& deltaSeconds, const FVector2& dirNormal);
	void AttackAction(const FVector2& bodyPos, const FVector2& worldMousePos, const FVector2& dirNormal);
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	void Input();

	EDroneType GetDroneType() const{ return droneType;}
	void SetDroneType(EDroneType type){ droneType = type;}
	float GetAttackDelay() const { return attackDelay; }
	void SetAttackDelay(float val) { attackDelay = val; }


	gameObject* m_player = nullptr;
	gameObject* m_owner = nullptr;

	SpriteRenderer* body = nullptr;
	SpriteRenderer* arm = nullptr;

	FVector2 initBodyPos = FVector2(-50.0f, 40.0f);
	FVector2 initBodySize = FVector2(0.7f, 0.7f);

	FTimerHandle timer;
	bool bCanFire = true;
	bool bWaitForSecond = true;

	// ���Ʒ� ������ �ӵ�
	float elapsed = 0.0f;
	float duration = 1.0f; // ����Ʒ� �� �� �̵��ϴµ� �ɸ��� �ð�
	float startY = 0.0f;
	float endY = 7.0f;    // ���� 5��ŭ �̵�
	bool goingUp = true;

	FDroneSpritePath spritePath;
	EDroneType droneType = EDroneType::Enemy;
	float attackDelay = 0.5f;

	// ���� �ٶ󺸴� �� ȸ�� �߰� ���� - �̹����� ȸ���Ǿ� ���� ��� �߰�.
	float armDegree = 0.0f;
};
