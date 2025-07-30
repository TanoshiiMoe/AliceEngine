#include "pch.h"
#include "TimerManager.h"

void TimerManager::Initialize()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&initCounter);
	prevCounter = initCounter;
}

void TimerManager::UpdateTime()
{
	QueryPerformanceCounter(&currentCounter);
	deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
	prevCounter = currentCounter;
	accumulator += deltaTime;
	//OutputDebugStringW((L"DeltaTime : " + std::to_wstring(deltaTime) + L"\n").c_str());

	frameCount++;
	timeSinceLastFps += deltaTime;
	if (timeSinceLastFps >= 1.0f)
	{
		currentFps = frameCount / timeSinceLastFps;
		frameCount = 0;
		timeSinceLastFps = 0.0f;
		ShowFPSDebug();
	}
}

void TimerManager::UpdateFixedTime(std::function<void(const float&)> f)
{
	while (accumulator >= fixedDeltaTime)
	{
		float dt = min(fixedDeltaTime, accumulator);

		fixedTime += fixedDeltaTime;
		accumulator -= fixedDeltaTime;

		// 실제 FixedUpdate 로직 호출
		f(dt);
	}
	//OutputDebugStringW((L"accumulator : " + std::to_wstring(accumulator) + L"\n").c_str());
	//OutputDebugStringW((L"fixedTime : " + std::to_wstring(fixedTime) + L"\n").c_str());
}

bool TimerManager::IfFixedUpdatable()
{
	return accumulator >= fixedDeltaTime;
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

float TimerManager::GetFixedTime()
{
	return fixedTime;
}

float TimerManager::GetAccumulator() const
{
	return accumulator;
}

float TimerManager::GetCurrentFPS() const
{
	return currentFps;
}

void TimerManager::ShowFPSDebug()
{

	OutputDebugStringW((L"FPS : " + std::to_wstring(currentFps) + L"\n").c_str());
}