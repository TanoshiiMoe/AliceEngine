#include "pch.h"
#include "gameObject.h"
#include "../Application.h"

gameObject::gameObject(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
{
	Initialize(position, rotation, scale, pivot);
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

void gameObject::Update()
{
	
}

void gameObject::Release()
{
	
}