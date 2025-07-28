#include "pch.h"
#include "BoxComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Math/TMath.h>
#include <Math/TColor.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>

BoxComponent::BoxComponent()
{
	SetColor(FColor::Black);
}

BoxComponent::BoxComponent(const FVector2& _size = FVector2(50,50), const FColor& color = FColor::Black)
{
	SetSize(_size);
	SetColor(color);
}

BoxComponent::~BoxComponent()
{
	m_pBrush = nullptr;
}

void BoxComponent::Initialize()
{
	__super::Initialize();

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);
}

void BoxComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void BoxComponent::Release()
{
	__super::Initialize();
}

void BoxComponent::Render()
{
	if (GetOwner() == nullptr) return;

	__super::Render();

	FVector2 scale(1.0f, 1.0f);
	if (bIgnoreOwnerScale) {
		if (auto transformComp = GetOwner()->transform())
			scale = transformComp->GetScale();
	}
	// 스케일 무시 모드면 scale은 (1,1) 유지

	float drawWidth = m_size.x / (scale.x != 0 ? scale.x : 1.0f);
	float drawHeight = m_size.y / (scale.y != 0 ? scale.y : 1.0f);

	D2D1_POINT_2F pivot = 
	{
		drawWidth * GetPivot()->x,
		drawHeight * GetPivot()->y
	};
	D2DRenderManager::GetD2DDevice()->DrawRectangle(
		D2D1::RectF(-pivot.x, -pivot.y, pivot.x, pivot.y),
		m_pBrush.Get(), thickness
	);
}

float BoxComponent::GetBitmapSizeX()
{
	return m_size.x;
}

float BoxComponent::GetBitmapSizeY()
{
	return m_size.y;
}

void BoxComponent::SetColor(const FColor& color)
{
	m_color = color;
	m_pBrush = nullptr;
	D2DRenderManager::GetInstance().m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}
