#pragma once
#include <Core/Singleton.h>
#include <Windows.h>

class UIManager : public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void Initialize();
	void Update();
	void Render();
};

