#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Collider.h"
#include "TileMap/TileMapComponent.h"

class TileMapManager : public ScriptComponent
{
public:
	static TileMapManager* instance;

	void Initialize() override;
	void OnStart() override;

	TileMapComponent* GetTileMapComponent();
private:
	Collider* spawnerCollider = nullptr;
};

