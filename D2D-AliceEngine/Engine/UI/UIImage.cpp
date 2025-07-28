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
* UIComponent에서 월드 좌표를 이미 구했기 때문에 여기서는 Image에 대한 것만 렌더링.
* 추가로 destRect 적용해볼것.
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
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}