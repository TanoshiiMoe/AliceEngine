#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Collider.h"

class TileMapManager : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
private:
	Collider* spawnerCollider = nullptr;
};

