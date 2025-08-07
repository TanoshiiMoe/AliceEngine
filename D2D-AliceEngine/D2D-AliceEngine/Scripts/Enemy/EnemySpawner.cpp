#include "EnemySpawner.h"
#include "System/ScriptSystem.h"
#include "TileMap/TileMapComponent.h"
#include "Object/gameObject.h"
#include "Scene/Scene.h"
#include "Manager/SceneManager.h"

void EnemySpawner::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemySpawner::OnStart()
{
	// �׽�Ʈ�� �׳� ������ ����
	CreatSpawnerMap();
}

void EnemySpawner::CreatSpawnerMap(TileMapComponent* _tilemap /*= nullptr*/)
{
	if (_tilemap == nullptr) {
		WeakObjectPtr<gameObject> obj = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");
		if (obj) {
			_tilemap = obj->GetComponent<TileMapComponent>();
		}
		else
			OutputDebugStringW(L"EnemySpawner���� TileMap�� ã���� ����!!!!");
	}
}
