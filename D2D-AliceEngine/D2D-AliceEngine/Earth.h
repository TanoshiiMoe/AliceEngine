#pragma once
#include "Sun.h"
class Earth : public Sun
{
public:
	Earth()
	{
		LoadData();
		Initialize();
	};
	~Earth() {};

	void Initialize() override
	{
		__super::Initialize();
		m_transform->SetPosition(100.0f, 100.0f);
		m_transform->SetScale(1.0f, 1.0f);
	}

	void Release() override
	{
		__super::Release();
	}

	void LoadData() override
	{
		__super::LoadData();
		m_bitmapImage.LoadData(L"Mushroom.png");
	}
};

