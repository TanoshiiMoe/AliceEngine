#include "pch.h"
#include "CoordHelper.h"

FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _pos)
{
	// UI (0~1) �� �ȼ� ��ǥ�� ��ȯ, Y�� ������
	return {
		_pos.x * static_cast<float>(Define::SCREEN_WIDTH),
		_pos.y * static_cast<float>(Define::SCREEN_HEIGHT)
	};
}