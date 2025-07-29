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
	for (const auto& layer : tilemap.layers)
	{
		if (!layer.visible) continue; // ���̾ ��Ȱ��ȭ�� ��� �ǳʶٱ�
		for (int row = 0; row < layer.height; ++row)
		{
			for (int col = 0; col < layer.width; ++col)
			{
				int index = row * layer.width + col;
				int gid = layer.data[index];
				if (gid == 0) continue; // �� Ÿ���� �ǳʶٱ�

				gameObject* go = GetWorld()->NewObject<gameObject>(L"tileSprite");
				//go->transform()->SetPosition(FVector2(row * tileset.tilewidth, col * tileset.tileheight));
				go->transform()->SetPosition(FVector2(Define::SCREEN_WIDTH * (((float)col / layer.width) - 0.5f), Define::SCREEN_HEIGHT * (0.5f - ((float)row / layer.height))));
				//go->transform()->SetPosition(FVector2(-Define::SCREEN_WIDTH / 2.0f + col,0));
				TileMapRenderer* tileRenderer = go->AddComponent<TileMapRenderer>();
				tileRenderer->LoadData(StringHelper::string_to_wstring(tileset.image));
				

				float gidRow = (gid-1) / tileset.columns ;
				float gidCol = (gid-1) % tileset.columns;
				float x = gidCol * tileset.tilewidth;
				float y = gidRow * tileset.tileheight;
				float width = tileset.tilewidth;
				float height = tileset.tileheight;
				tileRenderer->SetSlice(x, y, width, height);
				
				// �� �κп��� bimap ���� �߶����
				//tileRenderer = std::make_shared<SpriteRenderer>();
				//tileRenderer->row = row;
				//tileRenderer->col = col;
				//tileRenderer->width = tileset.tilewidth;
				//tileRenderer->height = tileset.tileheight;

				//// ��������Ʈ ������ ���� �� ����
				//auto spriteRenderer = std::make_shared<SpriteRenderer>();
				//spriteRenderer->LoadData(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + StringHelper::string_to_wstring(tileset.image)));
				//spriteRenderer->SetTransform(GetTransformComp());
				//spriteRenderer->SetPivot(GetPivot());
				//spriteRenderer->SetScale(FVector2(1.0f, 1.0f));
				//tileRenderer->spriteRenderer = spriteRenderer;

				m_TileRenderers.push_back(tileRenderer);
			}
		}
	}
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

//WeakObjectPtr<TileMapWrapper> TileMapComponent::AddSpriteRenderer(const std::wstring& path)
//{
//	SpriteRenderer* sr = GetOwner()->AddComponent<SpriteRenderer>();
//	return WeakObjectPtr<SpriteRenderer>(sr);
//
//	//sr->LoadData(path);
//	//sr->SetTransform(GetTransform());
//	//sr->SetPivot(GetPivot());
//	//sr->Initialize();
//	//m_TileRenderers.push_back(sr);
//	//
//	//SceneManager::GetInstance().GetCurrentScene()->AddComponent(spriteRenderer);
//}

float TileMapComponent::GetBitmapSizeX()
{
	return 0;
}

float TileMapComponent::GetBitmapSizeY()
{
	return 0;
}

FVector2 TileMapComponent::GetSize()
{
	return FVector2(0, 0); // TODO: ���� ũ�� ��� ���� �߰�)
}