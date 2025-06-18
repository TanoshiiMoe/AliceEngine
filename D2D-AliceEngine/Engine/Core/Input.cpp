#include "pch.h"
#include "Input.h"

namespace Input
{
	SHORT prevState[256] = { 0 };
	SHORT currState[256] = { 0 };
	HWND m_hwnd;
	POINT m_point;

	void Initialize(HWND hwnd)
	{
		m_hwnd = hwnd;
	}

	void Update()
	{
		memcpy_s(prevState, sizeof(prevState), currState, sizeof(currState));
		for (int i = 0; i < 256; i++)
		{
			currState[i] = GetAsyncKeyState(i);
		}

		GetCursorPos(&m_point);
		ScreenToClient(m_hwnd, &m_point);
	}

	bool IsKeyDown(int vKey)
	{
		return (currState[vKey] & 0x8000) != 0;
	}

	bool IsKeyPressed(int vKey)
	{
		return (!(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000));
	}

	bool IsKeyReleased(int vKey)
	{
		return ((prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000));
	}

	FVector2 GetMousePosition()
	{
		return FVector2(float(m_point.x), float(m_point.y));
	}

	//FVector2 GetMouseWorldPosition(std::weak_ptr<ACameraActor> camera)
	//{
	//	FVector2 screenPos = GetMousePosition();
	//	FVector2 cameraPos = FVector2(0, 0);
	//	if (const auto cameraRef = camera.lock())
	//	{
	//		cameraPos = cameraRef->GetActorLocation();
	//	}
	//	return screenPos + cameraPos;
	//}
}