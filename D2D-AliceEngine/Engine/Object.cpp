#include "pch.h"
#include "Object.h"
#include "Application.h"

void Object::Initialize()
{
	if (m_localTransform == nullptr)
	{
		m_localTransform = new Transform();
	}
	if (m_worldTransform == nullptr)
	{
		m_worldTransform = new Transform();
	}
}

void Object::LoadBitmapData(const std::wstring& path)
{
	HRESULT hr = Application::GetInstance()->m_pD2DRenderer->CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));
}

void Object::Release()
{
	m_localTransform = nullptr;
	m_worldTransform = nullptr;
	m_bitmap = nullptr;
}
