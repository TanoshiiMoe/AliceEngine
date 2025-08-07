#pragma once
#include <Core/Singleton.h>
#include <Windows.h>
#include <functional>

class FTimerHandle
{
public:
	size_t InternalHandle = 0;

	bool operator==(const FTimerHandle& Other) const { return InternalHandle == Other.InternalHandle; }
	bool operator!=(const FTimerHandle& Other) const { return !(*this == Other); }
};

struct FTimerSlot
{
	FTimerHandle handle;
	std::function<void()> callback;
	float remainingTime = 0.0f;
	float loopTime = 0.0f;
	bool looping = false;
	bool active = true;
};

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
	float GetGlobalTimeScale() const;
	void SetGlobalTimeScale(const float& _value);

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

	float globalTimeScale = 1.0f;


public:

	// �Ϲ� SetTimer
	template <typename UserClass>
	void SetTimer(
		FTimerHandle& OutHandle,
		UserClass* InObj,
		void (UserClass::* InFunc)(),
		float Rate,
		bool bLoop = false,
		float FirstDelay = 0.0f
	);

	void ClearTimer(FTimerHandle Handle);
	bool IsTimerActive(FTimerHandle Handle) const;
	void PauseTimer(FTimerHandle Handle);
	void UnPauseTimer(FTimerHandle Handle);
	float GetTimerElapsed(FTimerHandle Handle) const;
	float GetTimerRemaining(FTimerHandle Handle) const;
	void SetTimer(FTimerHandle& OutHandle, std::function<void()> InCallback, float Rate, bool bLoop, float FirstDelay);

private:
	struct TimerData
	{
		std::function<void()> Callback;
		float TimeRemaining = 0.0f;
		float OriginalRate = 0.0f;
		bool bLooping = false;
		bool bPaused = false;
		float Elapsed = 0.0f;
	};

	std::unordered_map<size_t, TimerData> Timers;
	size_t NextHandle = 1;
};

