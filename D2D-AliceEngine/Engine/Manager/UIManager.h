#pragma once
#include <Core/Singleton.h>
#include <Windows.h>

// ������ ��� : UIButton / UIText
class UIManager : public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void Initialize();
	void Update();
	void Render();
};

