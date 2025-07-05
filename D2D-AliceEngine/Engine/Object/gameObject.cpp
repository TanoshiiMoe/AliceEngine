#include "pch.h"
#include "gameObject.h"
#include <Math/Transform.h>
#include <Component/SpriteRenderer.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <FSM/FiniteStateMachine.h>
#include <Manager/D2DRenderManager.h>

gameObject::gameObject(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
{
	Initialize(position, rotation, scale, pivot);
}

gameObject::~gameObject()
{
	for (std::weak_ptr<Component>&& comp : m_Components)
	{
		comp.lock()->OnDestroy();
	}
	m_Components.clear();
}

void gameObject::OnStart()
{
	for (std::weak_ptr<Component>&& comp : m_Components)
	{
		comp.lock()->OnStart();
	}
}

void gameObject::OnEnd()
{
	for (std::weak_ptr<Component>&& comp : m_Components)
	{
		comp.lock()->OnEnd();
	}
	m_Components.clear();
}

void gameObject::Initialize()
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(FVector2(0.0f), 0, FVector2(1.0f), FVector2(0.0f));
}

void gameObject::Initialize(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(position, rotation, scale, pivot);
}

void gameObject::AddChildObject(const gameObject* obj)
{
	this->transform()->AddChildObject(obj->m_transformComponent);
}

void gameObject::Update()
{

}

void gameObject::Release()
{
	
}