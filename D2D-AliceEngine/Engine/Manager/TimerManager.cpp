#include "pch.h"
#include "TimerManager.h"

void TimerManager::Initialize()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&initCounter);
}

void TimerManager::UpdateTime()
{
	QueryPerformanceCounter(&currentCounter);
	deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
	prevCounter = currentCounter;
}

float TimerManager::GetElapsedTime()
{
	return deltaTime;
}

float TimerManager::GetTotalTime()
{
	float totalTime = static_cast<float>(currentCounter.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
	return totalTime;
}
