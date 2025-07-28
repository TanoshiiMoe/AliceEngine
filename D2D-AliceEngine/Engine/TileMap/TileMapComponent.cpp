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

TileMapComponent::TileMapComponent()
{
}

TileMapComponent::~TileMapComponent()
{
	for(auto& tileRenderer : m_TileRenderers)
	{
		if (auto renderer = tileRenderer.Get())
		{
			renderer->spriteRenderer.reset();
		}
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
	std::wstring filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	TileMapLoader::LoadTileMap(filePath, tilemap);
}

void TileMapComponent::LoadTileSetData(const std::wstring& path)
{
	std::wstring filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	TileMapLoader::LoadTileSet(filePath, tileset);
}

void TileMapComponent::LoadMapData(const std::wstring& path)
{
	//filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	//m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
	//	(Define::BASE_RESOURCE_PATH + path).c_str());
}

void TileMapComponent::CreatetileRenderers()
{
	//for (const auto& layer : tilemap.layers)
	//{
	//	if (!layer.visible) continue; // ���̾ ��Ȱ��ȭ�� ��� �ǳʶٱ�
	//	for (int row = 0; row < layer.height; ++row)
	//	{
	//		for (int col = 0; col < layer.width; ++col)
	//		{
	//			int index = row * layer.width + col;
	//			int gid = layer.data[index];
	//			if (gid == 0) continue; // �� Ÿ���� �ǳʶٱ�
	//
	//			SpriteRenderer* tileRenderer = GetOwner()->AddComponent<SpriteRenderer>();
	//
	//			// �� �κп��� bimap ���� �߶����
	//
	//			auto tileRenderer = std::make_shared<TileMapWrapper>();
	//			tileRenderer->row = row;
	//			tileRenderer->col = col;
	//			tileRenderer->width = tileset.tilewidth;
	//			tileRenderer->height = tileset.tileheight;
	//			// ��������Ʈ ������ ���� �� ����
	//			auto spriteRenderer = std::make_shared<SpriteRenderer>();
	//			spriteRenderer->LoadData(FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + tileset.image));
	//			spriteRenderer->SetTransform(GetTransform());
	//			spriteRenderer->SetPivot(GetPivot());
	//			spriteRenderer->SetScale(FVector2(1.0f, 1.0f));
	//			
	//			tileRenderer->spriteRenderer = spriteRenderer;
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
	//if (m_bitmap == nullptr) return;
	//__super::Render();
	//
	//// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	//D2D1_POINT_2F m_pivot =
	//{
	//	GetBitmapSizeX() * GetPivot()->x,
	//	GetBitmapSizeY() * GetPivot()->y
	//};
	//D2D1_RECT_F destRect = { -m_pivot.x, -m_pivot.y,  m_pivot.x,  m_pivot.y };
	//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
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