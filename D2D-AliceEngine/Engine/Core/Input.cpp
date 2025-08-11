#include "pch.h"
#include "Input.h"
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>

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

		// Ű���� ���� �ʱ�ȭ
		for (int i = 0; i < 256; i++)
		{
			currState[i] = GetAsyncKeyState(i);
			prevState[i] = currState[i];
		}

		// ���콺 ��ǥ �ʱ�ȭ
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
	
		// ���콺 ���� ���� �� �ڵ� �ʱ�ȭ
		// ���콺�� Ŭ���� ���·� �ʹ� ���� �����Ǵ� ��� �ڵ� �ʱ�ȭ
		static int mouseHoldFrameCount = 0;
		if (IsMouseLeftDown())
		{
			mouseHoldFrameCount++;
			// 300������(�� 5��) �̻� ���콺�� Ŭ���� ���·� �����Ǹ� �ʱ�ȭ
			if (mouseHoldFrameCount > 300)
			{
				ResetMouseState();
				mouseHoldFrameCount = 0;
			}
		}
		else
		{
			mouseHoldFrameCount = 0;
		}
	
		// �߰� ������ġ: ���콺�� Ŭ���� ���·� �ʹ� ���� �����Ǹ� ���� �ʱ�ȭ
		static int totalFrameCount = 0;
		totalFrameCount++;
	
		// �� 1000�����Ӹ��� ���콺 ���� ����
		if (totalFrameCount % 1000 == 0)
		{
			// ���콺�� Ŭ���� ���·� 1000������ �̻� �����Ǹ� ���� �ʱ�ȭ
			if (IsMouseLeftDown())
			{
				ResetMouseState();
			}
		}
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

	FVector2 GetMouseWorldPosition()
	{
		FVector2 mousePos = CoordHelper::ConvertD2DToUnity(GetMousePosition());
		FVector2 cameraWorldPos = SceneManager::GetInstance().GetCamera()->GetPosition();
		FVector2 worldMousePos = cameraWorldPos + mousePos;
		return worldMousePos;
	}

	FVector2 GetMouseWorldPositionInCanvas()
	{
		FVector2 mousePos = GetMousePosition();
		FVector2 cameraWorldPos = SceneManager::GetInstance().GetCamera()->GetPosition();
		FVector2 worldMousePos = cameraWorldPos + mousePos;
		return worldMousePos;
	}

	// �ƹ� Ű�� ������ ��
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
	
	void ResetMouseState()
	{
		// ���콺 Ŭ�� ���� �ʱ�ȭ
		prevState[VK_LBUTTON] = 0;
		currState[VK_LBUTTON] = 0;
		
		// ���콺 �� ���µ� �ʱ�ȭ
		m_mouseWheelDelta = 0;
		
		// ���콺 ��ġ�� ���� ��ġ�� ���� (Ŭ�� ���¸� �ʱ�ȭ)
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);
	}
}