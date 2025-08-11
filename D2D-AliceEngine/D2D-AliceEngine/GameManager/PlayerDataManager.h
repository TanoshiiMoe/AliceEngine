#pragma once
#include <Core/Singleton.h>
#include <string>

// �÷��̾� ���� ����ü
struct FPlayerStats
{
    int   maxHP       = 0;
    int   hp          = 0;
    float battery     = 0.0f;
    float moveSpeed   = 0.0f;
    float attackDelay = 0.0f;
    float damage      = 0.0f;
};

/*
 * @brief �÷��̾� ���� �����͸� �ε��ϰ� �����ϴ� �Ŵ���
 */
class PlayerDataManager : public Singleton<PlayerDataManager>
{
public:
    PlayerDataManager() = default;
    ~PlayerDataManager() = default;

    void LoadData(const std::wstring& relativePath);
    bool LoadFromJson(const std::wstring& relativePath);

    // �����Ͱ� �ε�Ǿ�����
    bool HasData() const { return m_loaded; }

    // ���� �������� (�ε� ���� �� �⺻�� ��ȯ)
    const FPlayerStats& GetStats() const { return m_stats; }

private:
    bool m_loaded = false;
    FPlayerStats m_stats;
};
