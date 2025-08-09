﻿#include "EnemySpawner.h"
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
#include <Component/Collider.h>
#include <Scripts/Bike/BikeStatScript.h>
#include <Scripts/Weapon/Drone.h>

EnemySpawner* EnemySpawner::instance = nullptr;

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
	if (instance == nullptr)
		instance = this;
	else
		SceneManager::GetInstance().GetWorld()->RemoveObject(owner.lock());
}

void EnemySpawner::SpawnEnemy(int _enemyTypeId /*= 0*/, FVector2 _position /*= {0.0f ,0.0f}*/)
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
		enemy->AddComponent<BikeStatScript>();
		
		if (Drone* drone = enemy->AddComponent<Drone>(dronePath))
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
		enemy->AddComponent<BikeStatScript>();

		if (Drone* drone = enemy->AddComponent<Drone>(dronePath))
		{
			drone->initBodyPos = FVector2(50.0f, 40.0f);
			drone->SetDroneType(EDroneType::Enemy);
		}
		break;
	default:
		break;
	}

	enemy->transform()->SetPosition(_position);
}


void EnemySpawner::SetStat(EnemyType type)
{
	switch (type)
	{
	case EnemySpawner::Bike:
		break;
	case EnemySpawner::Truck:
		break;
	case EnemySpawner::JTruck:
		break;
	case EnemySpawner::Car:
		break;
	case EnemySpawner::Boss:
		break;
	default:
		break;
	}
}
