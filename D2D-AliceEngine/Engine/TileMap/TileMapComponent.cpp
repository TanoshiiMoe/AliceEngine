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
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	TileMapLoader::LoadTileMap(filePath, tilemap);
}

void TileMapComponent::LoadTileSetData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	TileMapLoader::LoadTileSet(filePath, tileset);
}

void TileMapComponent::LoadMapData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
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
	//	if (!layer.visible) continue; // 레이어가 비활성화된 경우 건너뛰기
	//	int widthCount = 0;
	//	for (int row = 0; row < layer.height; ++row)
	//	{
	//		for (int col = 0; col < layer.width; ++col)
	//		{
	//			int index = row * layer.width + col;
	//			int gid = layer.data[index];
	//			if (gid == 0) continue; // 빈 타일은 건너뛰기
	//			if ((col * tileset.tilewidth) % Define::SCREEN_WIDTH == 0)
	//			{
	//				widthCount++;
	//			}
	//			gameObject* go = GetWorld()->NewObject<gameObject>(L"tileSprite");
	//			// x는 실제 위치를 그대로 그려야하고, y는 d2d->unity 좌표계 변환에서 y축 반전이 있으므로 절반에서 빼기
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
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
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
	return FVector2(0, 0); // TODO: 실제 크기 계산 로직 추가)
}