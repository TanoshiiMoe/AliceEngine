#include "EnemyManager.h"
#include "Manager/UpdateTaskManager.h"
#include "System/ScriptSystem.h"
#include "../Bike/LaneController.h"
#include <Helpers/StringHelper.h>
#include <Extension/json.hpp>
#include <fstream>
#include <Helpers/StringHelper.h>
#include <Helpers/FileHelper.h>

using json = nlohmann::json;

bool EnemyManager::s_statsLoaded = false;
std::unordered_map<int, FEnemyStats> EnemyManager::s_typeToStats;

void EnemyManager::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);

    // 적 스탯 JSON 로드 (최초 1회)
    EnsureStatsLoaded();
}

void EnemyManager::OnStart()
{

}

void EnemyManager::Update(const float& dt)
{
	if (isMoving)
		Move(dt);
}

void EnemyManager::Move(const float& dt)
{
	owner->transform()->AddPosition(moveSpeed * dt, 0.0f);
}

void EnemyManager::MoveUp()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveUp();
}

void EnemyManager::MoveDown()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveDown();
}

void EnemyManager::Die()
{
	dieActions.BroadCast();
}

void EnemyManager::AddDieAction(ObjectHandle _handle, std::function<void()> _func)
{
	dieActions.Add(_handle, _func);
}

void EnemyManager::RemoveDieAction(ObjectHandle _handle)
{
	dieActions.Remove(_handle);
}

void EnemyManager::ClearDieAction()
{
	dieActions.Clear();
}

void EnemyManager::EnsureStatsLoaded()
{
    if (s_statsLoaded) return;
    // 기본 경로: Resources/Enemy/EnemyData.json (없으면 무시)
    LoadStatsFromJson(Define::BASE_RESOURCE_PATH + L"Enemy/EnemyData.json");
    s_statsLoaded = true;
}

bool EnemyManager::LoadStatsFromJson(const std::wstring& relativePath)
{
    try
    {
        std::wstring filePath = FileHelper::ToAbsolutePath(relativePath);
        std::ifstream in(filePath);
        if (!in.is_open()) return false;
        json j; in >> j; in.close();
        if (!j.is_array()) return false;
        s_typeToStats.clear();
        for (const auto& entry : j)
        {
            int typeId = entry.value("typeId", -1);
            if (typeId < 0) continue;
            FEnemyStats st;
            st.maxHP       = entry.value("maxHP", 0);
            st.hp          = entry.value("hp", st.maxHP);
            st.battery     = entry.value("battery", 0.0f);
            st.moveSpeed   = entry.value("moveSpeed", 0.0f);
            st.attackDelay = entry.value("attackDelay", 0.0f);
            st.damage      = entry.value("damage", 0.0f);
            s_typeToStats[typeId] = st;
        }
        return true;
    }
    catch (...) { return false; }
}

bool EnemyManager::GetStats(int enemyTypeId, FEnemyStats& outStats)
{
    auto it = s_typeToStats.find(enemyTypeId);
    if (it == s_typeToStats.end()) return false;
    outStats = it->second; return true;
}

bool EnemyManager::HasStats(int enemyTypeId)
{
    return s_typeToStats.find(enemyTypeId) != s_typeToStats.end();
}
