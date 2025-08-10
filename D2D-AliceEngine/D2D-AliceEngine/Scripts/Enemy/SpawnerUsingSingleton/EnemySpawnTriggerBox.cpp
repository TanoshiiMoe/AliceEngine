#include "EnemySpawnTriggerBox.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Scripts/Enemy/Spawn/EnemySpawner.h>
#include <GameManager/EnemyDataManager.h>
#include <Scripts/Weapon/Drone.h>
#include <Prefab/Enemy/EnemyBike.h>
#include <Scripts/Enemy/EnemyStatScript.h>
#include <Prefab/Enemy/NormalTruck.h>
#include <Prefab/Enemy/JumpTruck.h>
#include <Prefab/Enemy/NormalCar.h>
#include <Scripts/Enemy/Spawn/SpawnData.h>
#include <GameManager/BulletManager.h>

void EnemySpawnTriggerBox::Initialize()
{
    __super::Initialize();
    REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemySpawnTriggerBox::OnStart()
{
    m_collider = owner->AddComponent<Collider>();
    if (m_collider)
    {
        m_collider->SetBoxSize(m_boxSize);
        m_collider->SetLayer(m_boxLayer);
    }
    m_initialized = true;
}

void EnemySpawnTriggerBox::Update(const float& /*deltaSeconds*/)
{
    __super::Update(0.0f);

    if (auto player = BulletManager::GetInstance().GetPlayer())
    {
        owner->SetPosition(player->GetPosition());
    }
}

void EnemySpawnTriggerBox::SpawnEnemyAt(int _enemyTypeId, const FVector2& worldPos)
{
    // 이름 정하기
    std::wstring name = L"";
    switch (_enemyTypeId)
    {
    case 0: name = L"EnemyBike"; break;
    case 4: name = L"EnemyBoss"; break;
    default: name = L"Neutral"; break;
    }

    // 적 오브젝트 생성 및 공통 세팅
    gameObject* enemy = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(name);
    EnemySpawner::EnemyType etype = static_cast<EnemySpawner::EnemyType>(_enemyTypeId);

    enemy->AddComponent<Collider>()->SetBoxSize(FVector2(80, 80));

    FDroneSpritePath dronePath(
        L"Enemy/Drone/enermy_Drone_body.png",
        L"Enemy/Drone/enermy_Drone_arm.png"
    );

    switch (etype)
    {
    case EnemySpawner::Bike:
        enemy->SetTag(L"Enemy");
        enemy->AddComponent<EnemyBike>();
        if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
            statScript->SetEnemyTypeId(_enemyTypeId);
        if (auto* drone = enemy->AddComponent<Drone>(dronePath))
        {
            drone->initBodyPos = FVector2(-60.0f, 80.0f);
            drone->initBodySize = FVector2(1.2f, 1.2f);
            drone->SetDroneType(EDroneType::Enemy);
            drone->SetAttackDelay(2.0f);
        }
        break;
    case EnemySpawner::Truck:
        enemy->SetTag(L"Obstacle");
        enemy->AddComponent<NormalTruck>();
        break;
    case EnemySpawner::JTruck:
        enemy->SetTag(L"JumpTrigger");
        enemy->AddComponent<JumpTruck>();
        break;
    case EnemySpawner::Car:
        enemy->SetTag(L"Obstacle");
        enemy->AddComponent<NormalCar>();
        break;
    case EnemySpawner::Boss:
        enemy->SetTag(L"Enemy");
        if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
            statScript->SetEnemyTypeId(_enemyTypeId);
        if (auto* drone = enemy->AddComponent<Drone>(dronePath))
        {
            drone->initBodyPos = FVector2(50.0f, 40.0f);
            drone->SetDroneType(EDroneType::Enemy);
        }
        break;
    default:
        break;
    }

    enemy->transform()->SetPosition(worldPos);
}

void EnemySpawnTriggerBox::OnTriggerEnter2D(Collider* collider)
{
    if (!m_initialized || !collider) return;

    if (collider->GetOwner()->GetTag() == L"EnemySpawn")
    {
        CollData temp = collider->GetOwner()->GetComponent<SpawnData>()->GetCollData();
        int etype = temp.enemyType;

        SpawnEnemyAt(etype, collider->GetOwner()->GetPosition());
    }
}
