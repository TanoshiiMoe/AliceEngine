#include "pch.h"
#include "SpriteRenderer.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>

SpriteRenderer::SpriteRenderer()
{
	spriteInfo.height = -1;
	spriteInfo.width = -1;
	spriteInfo.x = 0;
	spriteInfo.y = 0;
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
	spriteInfo.name = "";
}

SpriteRenderer::~SpriteRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // ��Ʈ�� ��ε�
}

void SpriteRenderer::Initialize()
{
	__super::Initialize();
}

void SpriteRenderer::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void SpriteRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());

	spriteInfo.height = GetBitmapSizeY();
	spriteInfo.width = GetBitmapSizeX();
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
}

void SpriteRenderer::Release()
{
	__super::Release();
}

/*
* @briefs : ��ȯ����� �����Ͽ� ��Ʈ���� ���ϴ� ��ġ�� �׸��ϴ�.
* @details
*	ETransformType : ��ǥ�� ����
*/

void SpriteRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();

	// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	/*float x = spriteInfo.x;
	float y = spriteInfo.y;
	float width = spriteInfo.width;
	float height = spriteInfo.height;
	D2D1_RECT_F SrcRect = { -width, height, width, -height};
 	D2D1_RECT_F destRect = { -width * spriteInfo.pivotX, -height * spriteInfo.pivotY,  -width * spriteInfo.pivotX + width,  -height * spriteInfo.pivotY + height };
	*/

	// �߶�� ���� ���� �� ���� -1�̸� ���� ����
	float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	float srcL = slice.srcX;
	float srcT = slice.srcY;

	D2D1_RECT_F srcRect = { srcL,               srcT,
							 srcL + cropW,       srcT + cropH };

	// ȭ�鿡 �׸� ��ġ(���͸� ����)
	float offsetX = -cropW * spriteInfo.pivotX;
	float offsetY = -cropH * spriteInfo.pivotY;

	D2D1_RECT_F destRect = { spriteInfo.x + offsetX,
							 spriteInfo.y + offsetY,
							 spriteInfo.x + offsetX + cropW,
							 spriteInfo.y + offsetY + cropH };
	
	// ����Ʈ ������ ����Ʈ �׸���
	if (!m_effect)
		D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
	else {
		D2D1_POINT_2F destPos = D2D1::Point2F(offsetX, offsetY);
		D2DRenderManager::GetD2DDevice()->DrawImage(m_effect.Get(), &destPos, &srcRect);
	}
		
}

float SpriteRenderer::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float SpriteRenderer::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 SpriteRenderer::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}

void SpriteRenderer::SetSlice(float x, float y, float w, float h)
{
	slice.srcX = x;  slice.srcY = y;
	slice.srcW = w;  slice.srcH = h;
}