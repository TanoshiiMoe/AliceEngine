#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::Initialize()
{
	TransformSystem::Get().Regist(this->weak_from_this());
	m_localTransform = new Transform();
	m_worldTransform = new Transform();
}

void TransformComponent::Release()
{
	delete m_localTransform;
	delete m_worldTransform;
	m_localTransform = nullptr;
	m_worldTransform = nullptr;

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->parent.reset();
		}
	}
}

void TransformComponent::Update()
{
	D2D1::Matrix3x2F mat;

	if (parent.lock())
	{
		mat = m_localTransform->ToMatrix() * parent.lock()->m_worldTransform->ToMatrix();
	}
	else
	{
		mat = m_localTransform->ToMatrix();
	}

	m_worldTransform->SetFromMatrix(mat);

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->Update();
		}
	}
}

void TransformComponent::SetTransform(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	m_localTransform->SetPosition(position.x, position.y);
	m_localTransform->SetRotation(rotation);
	m_localTransform->SetScale(scale.x, scale.y);
	SetPivot(pivot.x, pivot.y);
}
