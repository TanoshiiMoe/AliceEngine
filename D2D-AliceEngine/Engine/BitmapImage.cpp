#include "pch.h"
#include "BitmapImage.h"
#include "Application.h"

void BitmapImage::Initialize()
{

}

void BitmapImage::LoadData(const std::wstring& path)
{
	HRESULT hr = Application::GetInstance()->m_pD2DRenderer->CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));

}

void BitmapImage::Release()
{
	m_bitmap = nullptr;
}
