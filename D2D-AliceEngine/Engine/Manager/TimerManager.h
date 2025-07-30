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

	int frameCount = 0;          // 1�� ���� ������ ������ ��
	float timeSinceLastFps = 0.0f; // �ֱ� FPS ���� �� ����ð�
	float currentFps = 0.0f;     // ���� FPS ��

	float timeSinceLastDebug = 0.0f;
};

