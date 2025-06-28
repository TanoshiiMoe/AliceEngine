#include "pch.h"
#include "Camera.h"
#include "gameObject.h"
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

void Camera::Initialize()
{
	m_transform = std::make_shared<Transform>();
}

void Camera::Update()
{
	if (owner)
	{
		SetPosition(
			owner->transform()->m_localTransform->GetPosition().x,
			owner->transform()->m_localTransform->GetPosition().y
		)
;
	}
}

void Camera::Release()
{
	m_transform = nullptr;
	ClearOwner();
}

void Camera::SetPosition(const float& _x, const float& _y)
{
	m_transform->SetPosition(_x, _y);
}

FVector2 Camera::GetPosition()
{
	return FVector2(m_transform->GetPosition().x, m_transform->GetPosition().y);
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
