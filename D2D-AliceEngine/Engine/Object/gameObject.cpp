#include "pch.h"
#include "gameObject.h"
#include "../Application.h"

void gameObject::Initialize()
{
	
}

void gameObject::Initialize(const std::wstring& path, const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	m_bitmapRenderer = AddComponent<BitmapRenderer>();
	m_transformComponent = AddComponent<TransformComponent>();
	m_transformComponent.lock()->SetTransform(position, rotation, scale, pivot);
	m_bitmapRenderer.lock()->m_pTransform = m_transformComponent.lock()->m_worldTransform;
	m_bitmapRenderer.lock()->m_pivot = &m_transformComponent.lock()->m_pivot;

	LoadData(path);
}


void gameObject::Update()
{
	
}

void gameObject::Release()
{
	
}

void gameObject::LoadData(const std::wstring& path)
{
	if (m_bitmapRenderer.lock())
	{
		m_bitmapRenderer.lock()->LoadData(path);
	}
}
