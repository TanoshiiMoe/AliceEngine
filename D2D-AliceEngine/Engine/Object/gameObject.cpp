#include "pch.h"
#include "gameObject.h"
#include "../Application.h"

void gameObject::Initialize()
{
	
}

void gameObject::Initialize(const std::wstring& path, const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	m_bitmapRenderer = AddComponent<BitmapRenderer>();
	m_localTransform = AddComponent<Transform>();
	m_worldTransform = AddComponent<Transform>();
	m_bitmapRenderer.lock()->m_pTransform = m_worldTransform.lock();
	m_bitmapRenderer.lock()->m_pivot = &m_pivot;

	LoadData(path);

	if (m_localTransform.lock())
	{
		m_localTransform.lock()->SetPosition(position.x, position.y);
		m_localTransform.lock()->SetRotation(rotation);
		m_localTransform.lock()->SetScale(scale.x, scale.y);
		SetPivot(pivot.x, pivot.y);
	}
}


void gameObject::Update()
{
	D2D1::Matrix3x2F mat;

	if (parent.lock())
	{
		mat = m_localTransform.lock()->ToMatrix() * parent.lock()->m_worldTransform.lock()->ToMatrix();
	}
	else
	{
		mat = m_localTransform.lock()->ToMatrix();
	}

	m_worldTransform.lock()->SetFromMatrix(mat);

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->Update();
		}
	}
}

void gameObject::Release()
{
	m_localTransform.reset();
	m_worldTransform.reset();

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->parent.reset();
		}
	}
}

void gameObject::LoadData(const std::wstring& path)
{
	if (m_bitmapRenderer.lock())
	{
		m_bitmapRenderer.lock()->LoadData(path);
	}
}
