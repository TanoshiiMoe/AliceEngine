#include "pch.h"
#include "Time.h"

LARGE_INTEGER frequency;
LARGE_INTEGER prevCounter;
LARGE_INTEGER currentCounter;
LARGE_INTEGER initCounter;
float deltaTime = 0;

namespace Time
{
	void Initialize()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&initCounter);
	}
	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter);
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
		prevCounter = currentCounter;
	}
	float GetElapsedTime()
	{
		return deltaTime;
	}
	float GetTotalTime()
	{
		float totalTime = static_cast<float>(currentCounter.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
		return totalTime;
	}
}
