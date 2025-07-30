#include "pch.h"
#include "Input.h"

namespace Input
{
	SHORT prevState[256] = { 0 };
	SHORT currState[256] = { 0 };
	HWND m_hwnd;
	POINT m_point;					// MousePos
	short m_mouseWheelDelta;		// MouseWheel

	void Initialize(HWND hwnd)
	{
		m_hwnd = hwnd;

		// 키보드 상태 초기화
		for (int i = 0; i < 256; i++)
		{
			currState[i] = GetAsyncKeyState(i);
			prevState[i] = currState[i];
		}

		// 마우스 좌표 초기화
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);
	}

	void Update()
	{
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);

		m_mouseWheelDelta = 0;

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

	// 아무 키를 눌렀을 때
	bool AnyKeyPressed()
	{
		for (int i = 0; i < 256; ++i)
		{
			if (!(prevState[i] & 0x8000) && (currState[i] & 0x8000))
				return true;
		}
		return false;
	}

	void SetMousePosition(HWND hWnd, int mousePosX, int mousePosY)
	{
		m_hwnd = hWnd;

		POINT pt = { mousePosX, mousePosY };
		ClientToScreen(m_hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
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

	bool IsMouseLeftReleased()
	{
		return ((prevState[VK_LBUTTON] & 0x8000) && !(currState[VK_LBUTTON] & 0x8000));
	}
	bool IsMouseLeftPressed()
	{
		return (!(prevState[VK_LBUTTON] & 0x8000) && (currState[VK_LBUTTON] & 0x8000));
	}
	bool IsMouseLeftDown()
	{
		return (currState[VK_LBUTTON] & 0x8000) != 0;
	}
}