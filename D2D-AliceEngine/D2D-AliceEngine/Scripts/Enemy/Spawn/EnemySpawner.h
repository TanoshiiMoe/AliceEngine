#pragma once
#include "Component/ScriptComponent.h"
#include <unordered_map>

class TileMapComponent;
class EnemySpawner : public ScriptComponent
{
public:
	static EnemySpawner* instance;

	enum EnemyType {
		Bike = 0,	// �� ����ũ
		Truck,		// �߸� Ʈ��
		JTruck,		// ���� Ʈ��
		Car,		// �߸� ������
		Boss		// �η� ����
	};

	void Initialize() override;
	void OnStart() override;

	void SpawnEnemy(int _enemyTypeId = 0, FVector2 _position = {0.0f ,0.0f});
	void SetStat(EnemyType type);
};

