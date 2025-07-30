#pragma once
#include <Core/Singleton.h>
#include <Windows.h>
#include <functional>

class TimerManager : public Singleton<TimerManager>
{
public:
	void Initialize();
	void UpdateTime();
	void UpdateFixedTime(std::function<void(const float&)> f);
	bool IfFixedUpdatable();
	float GetElapsedTime();
	float GetTotalTime();

	float GetFixedTime();

	float GetAccumulator() const;

	float GetCurrentFPS() const;

	void ShowFPSDebug();

	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	LARGE_INTEGER initCounter;
	float deltaTime{0};
	float fixedTime{0};

	float fixedDeltaTime{ 0.08 };
private:
	float accumulator{ 0.0f };

	int frameCount = 0;          // 1초 동안 누적된 프레임 수
	float timeSinceLastFps = 0.0f; // 최근 FPS 측정 후 경과시간
	float currentFps = 0.0f;     // 현재 FPS 값

	float timeSinceLastDebug = 0.0f;
};

