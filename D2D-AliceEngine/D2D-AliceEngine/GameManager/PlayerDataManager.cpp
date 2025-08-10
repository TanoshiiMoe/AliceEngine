// Implement PlayerDataManager
#include "PlayerDataManager.h"
#include <Define/Define.h>
#include <Extension/json.hpp>
#include <Helpers/FileHelper.h>
#include <fstream>

using json = nlohmann::json;

void PlayerDataManager::LoadData(const std::wstring& relativePath)
{
    if (m_loaded) return;
    LoadFromJson(Define::BASE_RESOURCE_PATH + relativePath);
    m_loaded = true; // �ߺ� �ε� ���� (����/���� ������� �ѹ��� �õ�)
}

bool PlayerDataManager::LoadFromJson(const std::wstring& relativePath)
{
    try
    {
        std::wstring filePath = FileHelper::ToAbsolutePath(relativePath);
        std::ifstream in(filePath);
        if (!in.is_open()) return false;
        json j; in >> j; in.close();
        if (!j.is_array() || j.empty()) return false;
        const auto& entry = j.front(); // �÷��̾� �����ʹ� �ϳ��� �ִٰ� ����
        m_stats.maxHP       = entry.value("maxHP", 0);
        m_stats.hp          = entry.value("hp", m_stats.maxHP);
        m_stats.battery     = entry.value("battery", 0.0f);
        m_stats.moveSpeed   = entry.value("moveSpeed", 0.0f);
        m_stats.attackDelay = entry.value("attackDelay", 0.0f);
        m_stats.damage      = entry.value("damage", 0.0f);
        return true;
    }
    catch (...)
    {
        return false;
    }
}
