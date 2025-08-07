#pragma once
class CoordHelper
{
public:
	static FVector2 RatioCoordToScreen(const FVector2& _pos);
	static FVector2 RatioCoordToScreen(const FVector2& _size, const FVector2& _pos);
	static D2D1_POINT_2F TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt);
	static D2D1_MATRIX_3X2_F GetSkewMatrix(const FVector2& _angle, const float& _height);
	static FVector2 ConvertD2DToUnity(const FVector2& d2dPos);
};

