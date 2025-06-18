#include "pch.h"
#include "BoxComponent.h"
#include "Manager/D2DRenderManager.h"

BoxComponent::BoxComponent(const FVector2& _size = FVector2(50,50), const FColor& color = FColor::Black)
{
	SetSize(_size);
	SetColor(color);
}

void BoxComponent::Initialize()
{
}

void BoxComponent::Release()
{
}

void BoxComponent::Render()
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	context->DrawRectangle(
		D2D1::RectF(0, 0,
			m_size.x, m_size.y), m_pBrush.Get(), 5.0f);
}

void BoxComponent::SetColor(const FColor& color)
{
	m_color = color;
	m_pBrush = nullptr;
	D2DRenderManager::Get().m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}
