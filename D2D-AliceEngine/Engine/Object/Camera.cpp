#include "pch.h"
#include "Camera.h"
#include "gameObject.h"
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

void Camera::Initialize()
{
	relativeTransform.Initialize();
	if (owner) owner->transform()->AddChildObject(&relativeTransform);
	relativeTransform.SetPosition(0, 0);
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
	ClearOwner();
}

FVector2 Camera::GetScale()
{
	return FVector2(relativeTransform.GetScale().x, relativeTransform.GetScale().y);
}

void Camera::SetScale(const FVector2& scale)
{
	// 0 ���� ���� 0.0001�� ���� (�Ǵ� 0���� �����ص� ��)
	float safeX = scale.x > 0.0f ? scale.x : 0.0001f;
	float safeY = scale.y > 0.0f ? scale.y : 0.0001f;
	relativeTransform.SetScale(safeX, safeY);
}

void Camera::SetPosition(const float& _x, const float& _y)
{
	relativeTransform.SetPosition(_x, _y);
}

FVector2 Camera::GetPosition()
{
	return FVector2(relativeTransform.GetPosition().x, relativeTransform.GetPosition().y);
}

float Camera::GetPositionX()
{
	return relativeTransform.GetPosition().x;
}

float Camera::GetPositionY()
{
	return relativeTransform.GetPosition().y;
}

void Camera::SetRotation(const float& _val)
{
	relativeTransform.SetRotation(_val);
}

void Camera::AddRotation(const float& _val)
{
	relativeTransform.SetRotation(relativeTransform.GetRotation() + _val);
}

void Camera::AddPosition(const float& _x, const float& _y)
{
	relativeTransform.SetPosition(relativeTransform.GetPosition().x + _x, relativeTransform.GetPosition().y + _y);
}

// ��� ��ǥ, ������, ȸ�� ���� �Լ���
void Camera::SetRelativePosition(const FVector2& _pos)
{
	relativeTransform.SetPosition(_pos.x, _pos.y);
}

void Camera::SetRelativeRotation(const float& _rotation)
{
	relativeTransform.SetRotation(_rotation);
}

void Camera::SetRelativeScale(const FVector2& _scale)
{
	relativeTransform.SetScale(_scale.x, _scale.y);
}

void Camera::AddRelativePosition(const FVector2& _pos)
{
	FVector2 currentPos = relativeTransform.GetPosition();
	relativeTransform.SetPosition(currentPos.x + _pos.x, currentPos.y + _pos.y);
}

void Camera::AddRelativeRotation(const float& _rotation)
{
	float currentRotation = relativeTransform.GetRotation();
	relativeTransform.SetRotation(currentRotation + _rotation);
}

void Camera::AddRelativeScale(const FVector2& _scale)
{
	FVector2 currentScale = relativeTransform.GetScale();
	relativeTransform.SetScale(currentScale.x + _scale.x, currentScale.y + _scale.y);
}

FVector2 Camera::GetRelativePosition() const
{
	FVector2 pos = relativeTransform.GetPosition();
	return FVector2(pos.x, pos.y);
}

float Camera::GetRelativeRotation()
{
	return relativeTransform.GetRotation();
}

FVector2 Camera::GetRelativeScale()
{
	FVector2 scale = relativeTransform.GetScale();
	return FVector2(scale.x, scale.y);
}

void Camera::SetOwner(gameObject* obj)
{
	owner = obj;
}

void Camera::ClearOwner()
{
	owner = nullptr;
}

// �θ�-�ڽ� ���� ���� �Լ���
void Camera::AddChildObject(gameObject* obj)
{
	if (obj && obj->transform())
	{
		relativeTransform.AddChildObject(obj->transform());
	}
}

void Camera::RemoveFromParent()
{
	relativeTransform.RemoveFromParent();
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
	mat = mat * relativeTransform.m_worldTransform.ToMatrix();

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

	mat = mat * relativeTransform.m_worldTransform.ToMatrix();
	mat = mat * D2D1::Matrix3x2F::Scale(fov * scale.x, fov * scale.y);
	mat = mat * D2D1::Matrix3x2F::Scale(1.f, -1.f);
	mat = mat * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);

	return TransformPoint(mat, world);
}

// ���� ��ġ���� ui ��ǥ��� ��ȯ�ϱ�
D2D1_POINT_2F Camera::ScreenToUICoord(const D2D1_POINT_2F& screen)
{
	// D2D�� �»���� (0,0), �Ʒ��� ���������� y ����
	// Unity UI�� ���ϴ��� (0,0), ���� �ö󰥼��� y ����
	return {
		screen.x / static_cast<float>(Define::SCREEN_WIDTH),
		1.0f - (screen.y / static_cast<float>(Define::SCREEN_HEIGHT))
	};
}

// UICoordToScreen({0.5f,0.5f}); �� ���� ��ȯ���� ���� ��ġ.
D2D1_POINT_2F Camera::UICoordToScreen(const D2D1_POINT_2F& ui)
{
	// UI (0~1) �� �ȼ� ��ǥ�� ��ȯ, Y�� ������
	return {
		ui.x * static_cast<float>(Define::SCREEN_WIDTH),
		(1.0f - ui.y) * static_cast<float>(Define::SCREEN_HEIGHT)
	};
}