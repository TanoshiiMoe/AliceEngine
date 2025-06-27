#pragma once
#include <Windows.h>

namespace Time
{
	void Initialize();
	void UpdateTime();
	float GetElapsedTime();
	float GetTotalTime();
}
