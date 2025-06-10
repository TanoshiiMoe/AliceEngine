#pragma once
#include "DemoGameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	DemoGameApp* pGame = new DemoGameApp();
	if (pGame != nullptr)
	{
		pGame->Initialize();
		pGame->Run();
		pGame->Uninitialize();
	}
	else
	{
		assert("DemoGame instance not created in WinMain!");
	}
}
