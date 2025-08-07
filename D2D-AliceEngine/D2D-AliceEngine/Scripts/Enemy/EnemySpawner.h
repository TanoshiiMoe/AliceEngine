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
		JumpTruck,
		Car,
		Boss
	};

	void Initialize();
	void OnStart();

	void CreatSpawnerMap(TileMapComponent* _tilemap = nullptr);
private:
	std::unordered_map<FVector2, EnemyType> spawnerMap;

};

