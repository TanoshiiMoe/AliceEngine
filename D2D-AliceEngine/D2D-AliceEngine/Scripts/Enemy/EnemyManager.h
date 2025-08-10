#pragma once
#include "Component/ScriptComponent.h"
#include "Core/Delegate.h"
#include <unordered_map>

// �� ���� ����ü�� EnemyDataManager���� ����/����
struct FEnemyStats;

class EnemyManager : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;

	void Update(const float& deltaSeconds) override;

public:
	float moveSpeed = 50.0f;
	float isMoving = true;

	void Move(const float& dt);
	void MoveUp();
	void MoveDown();
	void Die();

	void AddDieAction(ObjectHandle _handle, std::function<void()> _func);
	void RemoveDieAction(ObjectHandle _handle);
	void ClearDieAction();

    // �̵�/�ൿ ���ø� ���. �����ʹ� EnemyDataManager�� ���� ����
private:
	MultiDelegate<> dieActions;

    // ���� ĳ��
    static bool s_statsLoaded;
    static std::unordered_map<int, FEnemyStats> s_typeToStats;
};

