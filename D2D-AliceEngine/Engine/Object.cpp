#include "pch.h"
#include "Object.h"
#include "Application.h"

void Object::Initialize()
{
	if(m_bitmapRenderer == nullptr)
		m_bitmapRenderer = std::make_shared<BitmapRenderer>();
}

void Object::Initialize(const std::wstring& path, const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	Initialize();
	LoadData(path);
	m_bitmapRenderer->m_localTransform->SetPosition(position.x, position.y);
	m_bitmapRenderer->m_localTransform->SetRotation(rotation);
	m_bitmapRenderer->m_localTransform->SetScale(scale.x, scale.y);
	m_bitmapRenderer->SetPivot(pivot.x, pivot.y);
}

void Object::Update()
{
	if (m_bitmapRenderer)
	{
		m_bitmapRenderer->Update();
	}
}

void Object::Release()
{
	m_bitmapRenderer.reset();
}

void Object::LoadData(const std::wstring& path)
{
	if (m_bitmapRenderer)
	{
		m_bitmapRenderer->LoadBitmapData(path);
	}
}

void Object::Render()
{
	if (m_bitmapRenderer)
	{
		m_bitmapRenderer->Render();
	}
}
