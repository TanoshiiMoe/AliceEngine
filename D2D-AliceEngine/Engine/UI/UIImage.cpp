#include "pch.h"
#include "UIImage.h"
#include <Helpers/FileHelper.h>
#include <Manager/PackageResourceManager.h>

UIImage::UIImage()
{
}

UIImage::~UIImage()
{

}

void UIImage::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}