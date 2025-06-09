#pragma once
#include "stdafx.h"
class Sun : public Object
{
public:
	Sun() 
	{
		LoadData();
		Initialize();
	};
	~Sun() {};

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

