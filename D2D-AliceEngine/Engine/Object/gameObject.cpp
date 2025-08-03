#include "pch.h"
#include "gameObject.h"
#include <Math/Transform.h>
#include <Component/SpriteRenderer.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <FSM/FiniteStateMachine.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/ClassManager.h>
#include <Scene/Scene.h>

gameObject::gameObject(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
{
	Initialize(position, rotation, scale, pivot);
}

gameObject::~gameObject()
{
	for (auto& component : m_components)
	{
		if (WeakObjectPtr<Component> comp = component)
		{
			comp.lock()->OnDestroy();
			delete component;
		}
	}
	m_components.clear();
	m_transformComponent.reset();
}

void gameObject::OnStart()
{
	for (auto& component : m_components)
	{
		if (WeakObjectPtr<Component> comp = component)
			comp.lock()->OnStart();
	}
}

void gameObject::OnEnd()
{
	for (auto& component : m_components)
	{
		if (WeakObjectPtr<Component> comp = component)
		{
			comp.lock()->OnEnd();
		}
	}
	for (auto& component : m_components)
	{
		delete component;
	}
	m_components.clear();
}

void gameObject::Initialize()
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(FVector2(0.0f), 0, FVector2(1.0f), FVector2(0.5f));
}

void gameObject::Initialize(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.5f))
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(position, rotation, scale, pivot);
}

void gameObject::AddChildObject(const gameObject* obj)
{
	if (auto weakThis = WeakFromThis<gameObject>())
	{
		weakThis->transform()->AddChildObject(obj->m_transformComponent);
	}
}

void gameObject::RemoveFromParent()
{
	if (auto transformComp = m_transformComponent.lock())
	{
		// 부모로부터 자신을 제거
		transformComp->RemoveFromParent();
	}
}

void gameObject::Update()
{

}

void gameObject::Release()
{
	//RemoveComponent<TransformComponent>(m_transformComponent.Get());
}