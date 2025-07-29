#include "pch.h"
#include "UIComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Object/Canvas.h>
#include <vector>

UIComponent::UIComponent()
{
	drawType = Define::EDrawType::ScreenSpace;
}

UIComponent::~UIComponent()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
	m_transformComponent.reset();
}

void UIComponent::Initialize()
{
	__super::Initialize();
	m_transformComponent = new TransformComponent();
	m_transformComponent->SetCanvas(WeakFromThis<Canvas>());
	m_transformComponent->Initialize();
}

void UIComponent::Update()
{
	__super::Update();
}

void UIComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	for (auto* child : m_child)
		child->Update(deltaSeconds);
}

void UIComponent::Release()
{
	__super::Release();

	for (auto* child : m_child)
	{
		if (child)
			child->SetParentUI(nullptr);
	}

	m_child.clear();
}

void UIComponent::Render()
{
	__super::Render(); // view 계산

	D2D1::Matrix3x2F localTransform = m_transformComponent->m_localTransform.ToMatrix();
		//D2D1::Matrix3x2F::Scale(m_transformComponent->GetScale().x, m_transformComponent->GetScale().y) *
		//D2D1::Matrix3x2F::Rotation(m_transformComponent->GetRotation() * 180.0f / Define::PI) *
		//D2D1::Matrix3x2F::Translation(m_transformComponent->GetPosition().x, m_transformComponent->GetPosition().y);
	view = view * localTransform;

	//D2DRenderManager::GetD2DDevice()->SetTransform(view);
	//D2D1_RECT_F destRect = { 0, 0, scale.x, scale.y };
	//D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	//for (auto* child : m_child)
	//{
	//	child->Render();
	//}

	for (auto* child : m_child)
		child->Render();
}

float UIComponent::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float UIComponent::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 UIComponent::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}

WeakObjectPtr<TransformComponent> UIComponent::GetUITransform() const
{
	return m_transformComponent;
}

void UIComponent::AddChildUI(UIComponent* child)
{
	if (!child || child == this) return;

	if (std::find(m_child.begin(), m_child.end(), child) != m_child.end())
		return;

	if (child->m_parent.IsValid() && child->m_parent.Get() != this)
	{
		// 기존 부모 자식 리스트에서 제거
		auto* oldParent = child->m_parent.Get();
		auto& siblings = oldParent->m_child;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), child), siblings.end());
	}

	child->m_parent = this;

	m_child.push_back(child);

	if (auto weakThis = WeakFromThis<UIComponent>())
	{
		weakThis->GetUITransform()->AddChildObject(child->m_transformComponent);
	}
}

void UIComponent::SetParentUI(UIComponent* parent)
{
	m_parent = parent;
}

void UIComponent::SetPosition(const FVector2& pos)
{
	m_transformComponent->SetPosition(pos.x, pos.y);
}

FVector2 UIComponent::GetUIPivot()
{
	if (!m_transformComponent.expired())
		return *m_transformComponent.Get()->GetPivot();
	return FVector2(0.5f, 0.5f);
}
