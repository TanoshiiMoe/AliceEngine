#pragma once
#include "pch.h"

using namespace Microsoft::WRL;

class BitmapImage
{
public:
	BitmapImage() {};
	~BitmapImage() {};

	void Initialize();
	void LoadData(const std::wstring& path);
	void Release();

	ID2D1Bitmap1* GetBitmap() const
	{
		return m_bitmap.Get();
	}

	ComPtr<ID2D1Bitmap1> m_bitmap;
};

