#include "pch.h"
#include "UIImage.h"
#include <Helpers/FileHelper.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/D2DRenderManager.h>
#include <memory>

UIImage::UIImage()
{
}

UIImage::~UIImage()
{

}

/*
* UIComponent���� ���� ��ǥ�� �̹� ���߱� ������ ���⼭�� Image�� ���� �͸� ������.
* �߰��� destRect �����غ���.
*/
void UIImage::Render()
{
	if (!m_bitmap) return;
	__super::Render();
	//D2D1_RECT_F destRect = { 0, 0, scale.x, scale.y };
	D2DRenderManager::GetD2DDevice()->SetTransform(view);
	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get());
	//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void UIImage::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}