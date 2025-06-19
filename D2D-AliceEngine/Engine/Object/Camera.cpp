#include "pch.h"
#include "Camera.h"
#include "gameObject.h"
#include <Component/TransformComponent.h>

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