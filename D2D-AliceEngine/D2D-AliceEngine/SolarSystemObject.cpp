#include "SolarSystemObject.h"

void SolarSystemObject::Initialize()
{
	__super::Initialize();

	m_bitmapRenderer = AddComponent<BitmapRenderer>(L"bitmapRenderer");
}

void SolarSystemObject::Initialize(const std::wstring& path, const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	Initialize();
	LoadData(path);

	if (m_bitmapRenderer.lock())
	{
		m_bitmapRenderer.lock()->m_localTransform->SetPosition(position.x, position.y);
		m_bitmapRenderer.lock()->m_localTransform->SetRotation(rotation);
		m_bitmapRenderer.lock()->m_localTransform->SetScale(scale.x, scale.y);
		m_bitmapRenderer.lock()->SetPivot(pivot.x, pivot.y);
	}
}

void SolarSystemObject::Update()
{

}

void SolarSystemObject::Release()
{
}

void SolarSystemObject::Render()
{
}

void SolarSystemObject::LoadData(const std::wstring& path)
{
	if (m_bitmapRenderer.lock())
	{
		m_bitmapRenderer.lock()->LoadBitmapData(path);
	}
}
