#include "pch.h"
#include "Camera.h"
#include "gameObject.h"
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

void Camera::Initialize()
{
	m_transform = std::make_unique<Transform>();
	m_transform->SetPosition(0, 0);
}

void Camera::Update()
{
	if (owner)
	{
		SetPosition(
			owner->transform()->m_localTransform.GetPosition().x,
			owner->transform()->m_localTransform.GetPosition().y
		);
	}
}

void Camera::Release()
{
	m_transform = nullptr;
	ClearOwner();
}

FVector2 Camera::GetScale()
{
	return FVector2(m_transform->GetScale().x, m_transform->GetScale().y);
}

void Camera::SetScale(const FVector2& scale)
{
	// 0 이하 값은 0.0001로 고정 (또는 0으로 고정해도 됨)
	float safeX = scale.x > 0.0f ? scale.x : 0.0001f;
	float safeY = scale.y > 0.0f ? scale.y : 0.0001f;
	m_transform->SetScale(safeX, safeY);
}

void Camera::SetPosition(const float& _x, const float& _y)
{
	m_transform->SetPosition(_x, _y);
}

FVector2 Camera::GetPosition()
{
	return FVector2(m_transform->GetPosition().x, m_transform->GetPosition().y);
}

float Camera::GetPositionX()
{
	return m_transform->GetPosition().x;
}

float Camera::GetPositionY()
{
	return m_transform->GetPosition().y;
}

void Camera::SetRotation(const float& _val)
{
	m_transform->SetRotation(_val);
}

void Camera::AddRotation(const float& _val)
{
	m_transform->SetRotation(m_transform->GetRotation() + _val);
}

void Camera::AddPosition(const float& _x, const float& _y)
{
	m_transform->SetPosition(m_transform->GetPosition().x + _x, m_transform->GetPosition().y + _y);
}

void Camera::SetOwner(gameObject* obj)
{
	owner = obj;
}

void Camera::ClearOwner()
{
	owner = nullptr;
}

D2D1_POINT_2F Camera::TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt)
{
	return {
		pt.x * mat.m11 + pt.y * mat.m21 + mat.dx,
		pt.x * mat.m12 + pt.y * mat.m22 + mat.dy
	};
}

D2D1_POINT_2F Camera::ViewportToWorldPoint(const D2D1_POINT_2F& viewport)
{
	float screenX = viewport.x * Define::SCREEN_WIDTH;
	float screenY = viewport.y * Define::SCREEN_HEIGHT;
	return ScreenToWorldPoint({ screenX, screenY });
}

D2D1_POINT_2F Camera::WorldToViewportPoint(const D2D1_POINT_2F& world)
{
	D2D1_POINT_2F screen = WorldToScreenPoint(world);
	return {
		screen.x / Define::SCREEN_WIDTH,
		screen.y / Define::SCREEN_HEIGHT
	};
}

D2D1_POINT_2F Camera::ScreenToWorldPoint(const D2D1_POINT_2F& screenPos)
{
	const float fov = fieldOfView;
	FVector2 scale = GetScale();
	if (scale.x == 0.0f) scale.x = 1.0f;
	if (scale.y == 0.0f) scale.y = 1.0f;

	D2D1_MATRIX_3X2_F mat = D2D1::Matrix3x2F::Identity();

	mat = mat * D2D1::Matrix3x2F::Scale(1.f, -1.f);
	mat = mat * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	mat = mat * D2D1::Matrix3x2F::Scale(fov * scale.x, fov * scale.y);
	mat = mat * m_transform->ToMatrix();

	BOOL success = D2D1InvertMatrix(&mat);

	return TransformPoint(mat, screenPos);
}

D2D1_POINT_2F Camera::WorldToScreenPoint(const D2D1_POINT_2F& world)
{
	const float fov = fieldOfView;
	FVector2 scale = GetScale();
	if (scale.x == 0.0f) scale.x = 1.0f;
	if (scale.y == 0.0f) scale.y = 1.0f;

	D2D1_MATRIX_3X2_F mat = D2D1::Matrix3x2F::Identity();

	mat = mat * m_transform->ToMatrix();
	mat = mat * D2D1::Matrix3x2F::Scale(fov * scale.x, fov * scale.y);
	mat = mat * D2D1::Matrix3x2F::Scale(1.f, -1.f);
	mat = mat * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);

	return TransformPoint(mat, world);
}

// 실제 위치에서 ui 좌표계로 변환하기
D2D1_POINT_2F Camera::ScreenToUICoord(const D2D1_POINT_2F& screen)
{
	// D2D는 좌상단이 (0,0), 아래로 내려갈수록 y 증가
	// Unity UI는 좌하단이 (0,0), 위로 올라갈수록 y 증가
	return {
		screen.x / static_cast<float>(Define::SCREEN_WIDTH),
		1.0f - (screen.y / static_cast<float>(Define::SCREEN_HEIGHT))
	};
}

// UICoordToScreen({0.5f,0.5f}); 로 쓰면 반환값이 실제 위치.
D2D1_POINT_2F Camera::UICoordToScreen(const D2D1_POINT_2F& ui)
{
	// UI (0~1) → 픽셀 좌표로 변환, Y축 뒤집기
	return {
		ui.x * static_cast<float>(Define::SCREEN_WIDTH),
		(1.0f - ui.y) * static_cast<float>(Define::SCREEN_HEIGHT)
	};
}