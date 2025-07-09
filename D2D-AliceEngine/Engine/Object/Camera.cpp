#include "pch.h"
#include "Camera.h"
#include "gameObject.h"
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

void Camera::Initialize()
{
	m_transform = std::make_unique<Transform>();
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
