#pragma once
#include <Core/Singleton.h>
#include <Windows.h>

class TimerManager : public Singleton<TimerManager>
{
public:
	void Initialize();
	void UpdateTime();
	float GetElapsedTime();
	float GetTotalTime();

	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	LARGE_INTEGER initCounter;
	float deltaTime = 0;
};

