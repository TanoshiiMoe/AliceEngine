#pragma once
#include <Windows.h>
namespace Input
{
	void Initialize(HWND hwnd);
	void Update();
	bool IsKeyDown(int vKey);
	bool IsKeyPressed(int vKey);
	bool IsKeyReleased(int vKey);
	bool AnyKeyPressed();
	FVector2 GetMousePosition();
	FVector2 GetMouseWorldPosition();
	FVector2 GetMouseWorldPositionInCanvas();
	void SetMousePosition(HWND hWnd, int mousePosX, int mousePosY);

	//FVector2 GetMouseWorldPosition(std::weak_ptr<ACameraActor> camera);
	bool IsMouseLeftReleased();
	bool IsMouseLeftPressed();
	bool IsMouseLeftDown();
}