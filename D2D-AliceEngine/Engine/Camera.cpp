#include "pch.h"
#include "Camera.h"

void Camera::Initialize()
{
	m_transform = std::make_shared<Transform>();
}

void Camera::Update()
{
}

void Camera::Release()
{
	m_transform = nullptr;
}