#include "pch.h"
#include "CoordHelper.h"
#include <Manager/D2DRenderManager.h>
#include <Application.h>
FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _pos)
{
	// UI (0~1) → 픽셀 좌표로 변환, Y축 뒤집기
	return {
		_pos.x * static_cast<float>(Application::GetInstance().GetSize().x),
		_pos.y * static_cast<float>(Application::GetInstance().GetSize().y)
	};
}

FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _size, const FVector2& _pivot)
{
	return {
		_pivot.x * static_cast<float>(_size.x),
		_pivot.y * static_cast<float>(_size.y)
	};
}

D2D1_POINT_2F CoordHelper::TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt)
{
	return D2D1::Point2F(
		pt.x * mat._11 + pt.y * mat._21 + mat._31,
		pt.x * mat._12 + pt.y * mat._22 + mat._32
	);
}

D2D1_MATRIX_3X2_F CoordHelper::GetSkewMatrix(const FVector2& _angle, const float& _height)
{
	float angleDeg = _angle.x; // 예: 30도
	float angleRad = angleDeg * (Define::PI / 180.0f);

	float totalHeight = _height;
	float xOffset = totalHeight * std::tan(angleRad);

	// 기준점: 왼쪽 아래 (붙어 있게 하려면 여기 기준으로 움직임)
	D2D1_MATRIX_3X2_F skewMatrix = D2D1::Matrix3x2F::Skew(angleDeg, 0);

	// 왼쪽 아래 기준, 오른쪽 위로 이동
	D2D1_MATRIX_3X2_F originTranslate = D2D1::Matrix3x2F::Translation(0, 0);
	D2D1_MATRIX_3X2_F skewTransform =
		originTranslate *
		skewMatrix *
		D2D1::Matrix3x2F::Translation(xOffset, 0);
	return skewTransform;
}

FVector2 CoordHelper::ConvertD2DToUnity(const FVector2& d2dPos)
{
	return FVector2(
		d2dPos.x - Define::SCREEN_WIDTH * 0.5f,
		-(d2dPos.y - Define::SCREEN_HEIGHT * 0.5f)
	);
}