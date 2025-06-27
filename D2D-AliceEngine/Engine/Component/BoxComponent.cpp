#include "pch.h"
#include "BoxComponent.h"
#include "Manager/D2DRenderManager.h"
#include <Manager/SceneManager.h>

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
	if (GetOwner() == nullptr) return;

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_POINT_2F pivotOffset = {
		m_size.x * m_pivot->x,
		m_size.y * m_pivot->y
	};
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = m_pTransform->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	cameraInv.Invert();
	view = view * world * cameraInv;

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	context->SetTransform(view);
	context->DrawRectangle(D2D1::RectF(0, 0, m_size.x, m_size.y), m_pBrush.Get(), 5.0f);
}

void BoxComponent::SetColor(const FColor& color)
{
	m_color = color;
	m_pBrush = nullptr;
	D2DRenderManager::GetInstance().m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}
