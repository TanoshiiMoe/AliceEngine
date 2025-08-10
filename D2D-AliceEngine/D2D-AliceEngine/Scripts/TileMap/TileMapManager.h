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

	// Ÿ�ϸ�, Ÿ�ϼ�, �浹 ������ ��θ� ����
	void SetTilePaths(const std::wstring& mapPath,
					  const std::wstring& setPath,
					  const std::wstring& collisionPath)
	{
		m_tileMapPath       = mapPath;
		m_tileSetPath       = setPath;
		m_tileCollisionPath = collisionPath;
	}

	TileMapComponent* GetTileMapComponent();
private:
	Collider* spawnerCollider = nullptr;
	// ��� ����
	std::wstring m_tileMapPath;
	std::wstring m_tileSetPath;
	std::wstring m_tileCollisionPath;
};

