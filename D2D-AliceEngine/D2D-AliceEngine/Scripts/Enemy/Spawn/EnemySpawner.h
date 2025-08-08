#pragma once
#include "Component/ScriptComponent.h"
#include <unordered_map>

class TileMapComponent;
class EnemySpawner : public ScriptComponent
{
public:
	enum EnemyType {
		Bike = 0,
		Truck,
		JTruck,
		Car,
		Boss
	};

	void Initialize();
	void OnStart();

	void SpawnEnemySkewPos(int _enemyTypeId = 0, FVector2 _position = {0.0f ,0.0f});
	void SetStat(EnemyType type);
};

