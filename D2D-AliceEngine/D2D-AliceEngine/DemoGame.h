#pragma once
#include "../Engine/Application.h"
#include "../Engine/Singleton.h"
#include "stdafx.h"

class DemoGame : public Application, public Singleton<DemoGame>
{
public:
	DemoGame();
	virtual ~DemoGame();
	static DemoGame* m_pInstance;
	static DemoGame* GetInstance()
	{
		assert(m_pInstance != nullptr && "DemoGame instance not created!");
		return m_pInstance;
	}

public:
	void Initialize() override;
	void Run() override;
	void Uninitialize() override;

public:
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

