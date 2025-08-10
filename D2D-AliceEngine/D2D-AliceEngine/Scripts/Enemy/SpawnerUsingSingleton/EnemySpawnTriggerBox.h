#pragma once
#include <Component/ScriptComponent.h>

class Collider;

class EnemySpawnTriggerBox : public ScriptComponent
{
public:
    void Initialize() override;
    void OnStart() override;
    void Update(const float& deltaSeconds) override;

    void OnTriggerEnter2D(Collider* collider) override;

    // 설정 API
    void SetBox(const FVector2& size, int layer = 1) { m_boxSize = size; m_boxLayer = layer; }

private:
    void SpawnEnemyAt(int _enemyTypeId, const FVector2& worldPos);
    Collider* m_collider{ nullptr };
    FVector2 m_boxSize{ 600.0f, 600.0f };
    int m_boxLayer{ 1 };
    bool m_initialized{ false };
};
