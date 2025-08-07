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
	accumulator += deltaTime * globalTimeScale;
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


	std::vector<size_t> TimersToRemove;

	for (auto& Pair : Timers)
	{
		auto& Data = Pair.second;

		if (Data.bPaused)
			continue;

		Data.TimeRemaining -= deltaTime;
		Data.Elapsed += deltaTime;

		if (Data.TimeRemaining <= 0.0f)
		{
			Data.Callback();

			if (Data.bLooping)
			{
				Data.TimeRemaining = Data.OriginalRate;
				Data.Elapsed = 0.0f;
			}
			else
			{
				TimersToRemove.push_back(Pair.first);
			}
		}
	}

	for (size_t Id : TimersToRemove)
	{
		Timers.erase(Id);
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

float TimerManager::GetGlobalTimeScale() const
{
	return globalTimeScale;
}

void TimerManager::SetGlobalTimeScale(const float& _value)
{
	globalTimeScale = _value;
}

// =================== TimeHandle ===================

void TimerManager::ClearTimer(FTimerHandle Handle)
{
	Timers.erase(Handle.InternalHandle);
}

bool TimerManager::IsTimerActive(FTimerHandle Handle) const
{
	auto it = Timers.find(Handle.InternalHandle);
	return it != Timers.end() && !it->second.bPaused;
}

void TimerManager::PauseTimer(FTimerHandle Handle)
{
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		it->second.bPaused = true;
}

void TimerManager::UnPauseTimer(FTimerHandle Handle)
{
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		it->second.bPaused = false;
}

float TimerManager::GetTimerElapsed(FTimerHandle Handle) const
{
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		return it->second.Elapsed;
	return -1.0f;
}

float TimerManager::GetTimerRemaining(FTimerHandle Handle) const
{
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		return it->second.TimeRemaining;
	return -1.0f;
}

template <typename UserClass>
void TimerManager::SetTimer(
	FTimerHandle& OutHandle,
	UserClass* InObj,
	void (UserClass::* InFunc)(),
	float Rate,
	bool bLoop,
	float FirstDelay
)
{
	static_assert(std::is_base_of<UObjectBase, UserClass>::value, "SetTimer requires UObjectBase-derived class");

	FTimerHandle Handle;
	Handle.InternalHandle = NextHandle++;

	TimerData Data;
	Data.Callback = [InObj, InFunc]() {
		(InObj->*InFunc)();
		};
	Data.TimeRemaining = FirstDelay > 0 ? FirstDelay : Rate;
	Data.OriginalRate = Rate;
	Data.bLooping = bLoop;

	Timers[Handle.InternalHandle] = Data;
	OutHandle = Handle;
}

void TimerManager::SetTimer(FTimerHandle& OutHandle, std::function<void()> InCallback, float Rate, bool bLoop, float FirstDelay)
{
	FTimerHandle Handle;
	Handle.InternalHandle = NextHandle++;

	TimerData Data;
	Data.Callback = InCallback;
	Data.TimeRemaining = FirstDelay > 0 ? FirstDelay : Rate;
	Data.OriginalRate = Rate;
	Data.bLooping = bLoop;

	Timers[Handle.InternalHandle] = Data;
	OutHandle = Handle;
}