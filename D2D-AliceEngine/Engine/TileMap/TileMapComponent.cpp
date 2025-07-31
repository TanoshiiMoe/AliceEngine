#include "pch.h"
#include "TileMapComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <Scene/Scene.h>
#include <Component/TileMapRenderer.h>

TileMapComponent::TileMapComponent()
{
}

TileMapComponent::~TileMapComponent()
{
	for(auto& tileRenderer : m_TileRenderers)
	{
		tileRenderer.reset();
	}
	m_TileRenderers.clear();
}

void TileMapComponent::Initialize()
{
	__super::Initialize();
}

void TileMapComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TileMapComponent::LoadTileMapData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	TileMapLoader::LoadTileMap(filePath, tilemap);
}

void TileMapComponent::LoadTileSetData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	TileMapLoader::LoadTileSet(filePath, tileset);
}

void TileMapComponent::LoadMapData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

void TileMapComponent::CreatetileRenderers()
{
	gameObject* go = GetWorld()->NewObject<gameObject>(L"tileSprite");
	TileMapRenderer* tileRenderer = go->AddComponent<TileMapRenderer>();
	tileRenderer->m_layer = 100;
	tileRenderer->LoadData(StringHelper::string_to_wstring(tileset.image));
	//tileRenderer->SetSkew(skewAngle);
	//tileRenderer->SetSkew(skewAngle);
	tileRenderer->SetMapInfo(tilemap, tileset);

	//for (const auto& layer : tilemap.layers)
	//{
	//	if (!layer.visible) continue; // ���̾ ��Ȱ��ȭ�� ��� �ǳʶٱ�
	//	int widthCount = 0;
	//	for (int row = 0; row < layer.height; ++row)
	//	{
	//		for (int col = 0; col < layer.width; ++col)
	//		{
	//			int index = row * layer.width + col;
	//			int gid = layer.data[index];
	//			if (gid == 0) continue; // �� Ÿ���� �ǳʶٱ�
	//			if ((col * tileset.tilewidth) % Define::SCREEN_WIDTH == 0)
	//			{
	//				widthCount++;
	//			}
	//			gameObject* go = GetWorld()->NewObject<gameObject>(L"tileSprite");
	//			// x�� ���� ��ġ�� �״�� �׷����ϰ�, y�� d2d->unity ��ǥ�� ��ȯ���� y�� ������ �����Ƿ� ���ݿ��� ����
	//			//int tileX = (float)col * tileset.tilewidth - 0.5f * Define::SCREEN_WIDTH;
	//			float tileX = tileset.tileheight * std::tan(skewAngle * (Define::PI / 180)) * (float)row + (float)col * tileset.tilewidth - 0.5f * Define::SCREEN_WIDTH;
	//			//float tileY = Define::SCREEN_HEIGHT * (0.5f - ((float)row / layer.height));
	//			float tileY = (layer.height - 1 - row) * tileset.tileheight - Define::SCREEN_HEIGHT * 0.5f;
	//			go->transform()->SetPosition(FVector2(tileX, tileY));
	//
	//			TileMapRenderer* tileRenderer = go->AddComponent<TileMapRenderer>();
	//			tileRenderer->LoadData(StringHelper::string_to_wstring(tileset.image));
	//			//tileRenderer->m_layer = row + 100;
	//			tileRenderer->SetSkew(true, FVector2(skewAngle, 0.0f));
	//			
	//			float gidRow = (gid-1) / tileset.columns;
	//			float gidCol = (gid-1) % tileset.columns;
	//			float x = gidCol * tileset.tilewidth;
	//			float y = gidRow * tileset.tileheight;
	//			float width = tileset.tilewidth;
	//			float height = tileset.tileheight;
	//			tileRenderer->SetSlice(x, y, width, height);
	//
	//			m_TileRenderers.push_back(tileRenderer);
	//		}
	//	}
	//}
}

void TileMapComponent::Release()
{
	__super::Release();
}

/*
* @briefs : ��ȯ����� �����Ͽ� ��Ʈ���� ���ϴ� ��ġ�� �׸��ϴ�.
* @details
*	ETransformType : ��ǥ�� ����
*/

void TileMapComponent::Render()
{
	
}

float TileMapComponent::GetBitmapSizeX()
{
	return 0;
}

float TileMapComponent::GetBitmapSizeY()
{
	return 0;
}

void TileMapComponent::SetSkew(const float& angle)
{
	skewAngle = angle;
}

FVector2 TileMapComponent::GetSize()
{
	return FVector2(0, 0); // TODO: ���� ũ�� ��� ���� �߰�)
}