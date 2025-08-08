#include "EnemySpawner.h"
#include "System/ScriptSystem.h"
#include "TileMap/TileMapComponent.h"
#include "Object/gameObject.h"
#include "Scene/Scene.h"
#include "Manager/SceneManager.h"
#include "SpawnCollider.h"
#include <Prefab/Enemy/EnemyBike.h>
#include "Component/SkewTransform.h"
#include "Prefab/Enemy/NormalTruck.h"
#include "Prefab/Enemy/JumpTruck.h"
#include "Prefab/Enemy/NormalCar.h"

void EnemySpawner::Initialize()
{
	__super::Initialize();

	// 플레이어한테 붙일 콜라이더 생성
	gameObject* coll = GetWorld()->NewObject<gameObject>(L"SpawnCollider");
	coll->AddComponent<SpawnCollider>();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemySpawner::OnStart()
{
	
}

void EnemySpawner::SpawnEnemySkewPos(int _enemyTypeId /*= 0*/, FVector2 _position /*= {0.0f ,0.0f}*/)
{
	// 이름 정하기
	std::wstring name = L"";
	switch (_enemyTypeId)
	{ 
	case 0:
		name = L"EnemyBike";
		break;
	case 4:
		name = L"EnemyBoss";
		break;
	default:
		name = L"Neutral";
		break;
	}

	// 적 스폰
	gameObject* enemy = GetWorld()->NewObject<gameObject>(name);
	EnemyType etype = static_cast<EnemyType>(_enemyTypeId);

	switch (etype)
	{
	case EnemySpawner::Bike:
		enemy->AddComponent<EnemyBike>();
		break;
	case EnemySpawner::Truck:
		enemy->AddComponent<NormalTruck>();
		break;
	case EnemySpawner::JTruck:
		enemy->AddComponent<JumpTruck>();
		break;
	case EnemySpawner::Car:
		enemy->AddComponent<NormalCar>();
		break;
	case EnemySpawner::Boss:
		break;
	default:
		break;
	}

	enemy->transform()->SetPosition(_position);
}
