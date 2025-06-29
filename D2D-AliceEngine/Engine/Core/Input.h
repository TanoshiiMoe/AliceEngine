#pragma once
#include <Windows.h>
namespace Input
{
	void Initialize(HWND hwnd);
	void Update();
	bool IsKeyDown(int vKey);
	bool IsKeyPressed(int vKey);
	bool IsKeyReleased(int vKey);
	FVector2 GetMousePosition();
	//FVector2 GetMouseWorldPosition(std::weak_ptr<ACameraActor> camera);
}