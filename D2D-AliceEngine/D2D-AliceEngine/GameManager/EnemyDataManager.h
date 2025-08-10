#pragma once
#include <Core/Singleton.h>
#include <unordered_map>
#include <string>

struct FEnemyStats
{
    int   maxHP      = 0;
    int   hp         = 0;
    float battery    = 0.0f;
    float moveSpeed  = 0.0f;
    float attackDelay= 0.0f;
    float damage     = 0.0f;
};

class EnemyDataManager : public Singleton<EnemyDataManager>
{
public:
    EnemyDataManager() = default;
    ~EnemyDataManager() = default;

    // 로드/조회
    void LoadData(const std::wstring& relativePath);
    bool LoadFromJson(const std::wstring& relativePath);
    bool GetStats(int enemyTypeId, FEnemyStats& outStats) const;
    bool HasStats(int enemyTypeId) const;

private:
    bool m_loaded = false;
    std::unordered_map<int, FEnemyStats> m_stats;
};


