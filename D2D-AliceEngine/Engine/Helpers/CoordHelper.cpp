#include "pch.h"
#include "CoordHelper.h"
#include <Manager/D2DRenderManager.h>
#include <Application.h>

// ���� ��� ���� �ʱ�ȭ
FVector2 CoordHelper::m_cameraOffset = FVector2(0, 0);
FVector2 CoordHelper::m_screenSize = FVector2(1920, 1080);

FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _pos)
{
	// UI (0~1) �� �ȼ� ��ǥ�� ��ȯ, Y�� ������
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
	float angleDeg = _angle.x; // ��: 30��
	float angleRad = angleDeg * (Define::PI / 180.0f);

	float totalHeight = _height;
	float xOffset = totalHeight * std::tan(angleRad);

	// ������: ���� �Ʒ� (�پ� �ְ� �Ϸ��� ���� �������� ������)
	D2D1_MATRIX_3X2_F skewMatrix = D2D1::Matrix3x2F::Skew(angleDeg, 0);

	// ���� �Ʒ� ����, ������ ���� �̵�
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

FVector2 CoordHelper::WorldToScreen(const FVector2& worldPos)
{
	// ���� ��ǥ�� ȭ�� ��ǥ�� ��ȯ
	// ī�޶� �������� ����Ͽ� ��ȯ
	FVector2 screenPos = worldPos - m_cameraOffset;
	
	// ȭ�� �߾��� �������� ��ȯ
	screenPos.x += m_screenSize.x * 0.5f;
	screenPos.y += m_screenSize.y * 0.5f;
	
	return screenPos;
}

FVector2 CoordHelper::ScreenToWorld(const FVector2& screenPos)
{
	// ȭ�� ��ǥ�� ���� ��ǥ�� ��ȯ
	FVector2 worldPos = screenPos;
	
	// ȭ�� �߾��� �������� ��ȯ
	worldPos.x -= m_screenSize.x * 0.5f;
	worldPos.y -= m_screenSize.y * 0.5f;
	
	// ī�޶� �������� ����Ͽ� ��ȯ
	worldPos += m_cameraOffset;
	
	return worldPos;
}

void CoordHelper::SetCameraOffset(const FVector2& offset)
{
	m_cameraOffset = offset;
}

void CoordHelper::SetScreenSize(const FVector2& size)
{
	m_screenSize = size;
}