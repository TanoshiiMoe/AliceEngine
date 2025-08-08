#include "TileMapManager.h"
#include "System/ScriptSystem.h"
#include "TileMap/TileMapComponent.h"
#include "Object/gameObject.h"
#include "Manager/D2DRenderManager.h"
#include "Manager/SceneManager.h"
#include <Scripts/Enemy/Spawn/SpawnCollider.h>

void TileMapManager::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);

	/*gameObject* coll = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(L"SpawnCollider");
	spawnerCollider = coll->AddComponent<Collider>();
	spawnerCollider->SetBoxSize(FVector2(00.0f, 500.0f));*/
}

void TileMapManager::OnStart()
{
	TileMapComponent* tc = owner->GetComponent<TileMapComponent>();

	if (tc) {
		tc->LoadTileMapData(L"TileMap/stage01_real/stage01_real.tmj");
		tc->LoadTileSetData(L"TileMap/stage01_real/Tile_Road.tsj");
		tc->LoadTileCollisionData(L"TileMap/TileMapColiderInfo.json");
		tc->SetSkew({ 45, 0 });
		tc->CreateTileRenderers();
		tc->CreateTileCollision();
		tc->SetTileLayer(-5000);	// 타일 레이어를 3으로 설정

		// 타일맵 위치 조절
		int t_height = tc->tileMap.tileHeight;
		int map_height = tc->tileMap.height;
		float yOffSet = t_height * ((float)map_height / 2.0f);

		FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
		FVector2 offSet = { 0.0f, -(screenSize.y / 2 - yOffSet) };

		gameObject* realTile = tc->m_tile;
		realTile->transform()->AddPosition(offSet.x, offSet.y);

		// 타일맵 콜라이더 위치 조절
		for (auto& obj : tc->go) {
			obj->transform()->AddPosition(offSet.x, offSet.y);
		}
	}
	else {
		std::wstring message = owner->GetName() + L" : TileMapManager에서 TileMapComponent를 가져오는데 실패했습니다!!!\n";
		OutputDebugStringW(message.c_str());
	}
}
