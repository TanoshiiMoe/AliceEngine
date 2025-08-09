#pragma once
#include "Component/ScriptComponent.h"
#include "Core/Delegate.h"
#include <unordered_map>

// 적 스탯 구조체: JSON으로부터 주입되는 데이터 보관
struct FEnemyStats
{
    int   maxHP      = 0;
    int   hp         = 0;
    float battery    = 0.0f;
    float moveSpeed  = 0.0f;
    float attackDelay= 0.0f;
    float damage     = 0.0f;
};

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

    // JSON 기반 적 데이터 파싱/조회 (정적 전역 캐시)
    static void EnsureStatsLoaded();
    static bool LoadStatsFromJson(const std::wstring& relativePath);
    static bool GetStats(int enemyTypeId, FEnemyStats& outStats);
    static bool HasStats(int enemyTypeId);
private:
	MultiDelegate<> dieActions;

    // 전역 캐시
    static bool s_statsLoaded;
    static std::unordered_map<int, FEnemyStats> s_typeToStats;
};

