#pragma once
#include "Component.h"
#include <System/TransformSystem.h>
#include <Math/Transform.h>
class TransformComponent : public Component
{
public:
	TransformComponent() 
	{
		m_localTransform = new Transform();
		m_worldTransform = new Transform();
	}
	~TransformComponent() 
	{
		TransformSystem::Get().UnRegist(this->weak_from_this());
		delete m_localTransform;
		delete m_worldTransform;
		m_localTransform = nullptr;
		m_worldTransform = nullptr;
	}
public:
	void Initialize() override; 
	void Release() override;
	void Update() override;

	void SetTransform(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

public:
	// 계층구조 관리
	std::weak_ptr<TransformComponent> parent;
	std::vector<std::weak_ptr<TransformComponent>> children;

	inline void AddChildObject(std::weak_ptr<TransformComponent> child)
	{
		auto childPtr = child.lock();
		if (!childPtr) return; // nullptr 체크

		auto self = std::dynamic_pointer_cast<TransformComponent>(this->weak_from_this().lock());
		std::weak_ptr<TransformComponent> weakSelf = self;

		childPtr->parent = weakSelf;
		children.push_back(childPtr);
	}

	FVector2 m_pivot{ 0,0 }; // 좌표 중심점
	Transform* m_localTransform; // Transform 컴포넌트
	Transform* m_worldTransform; // Transform 컴포넌트

	inline void SetPosition(const float& _x, const float& _y)
	{
		m_localTransform->SetPosition(_x, _y);
		SetDirty();
	}

	inline void SetRotation(const float& _val)
	{
		m_localTransform->SetRotation(_val);
		SetDirty();
	}

	inline void AddRotation(const float& _val)
	{
		m_localTransform->SetRotation(m_localTransform->GetRotation() + _val);
		SetDirty();
	}

	inline void AddPosition(const float& _x, const float& _y)
	{
		m_localTransform->SetPosition(m_localTransform->GetPosition().x + _x, m_localTransform->GetPosition().y + _y);
		SetDirty();
	}

	inline void SetPivot(const float& _x, const float& _y)
	{
		m_pivot.x = _x;
		m_pivot.y = _y;
	}
	inline void SetPivot(const float& _x)
	{
		m_pivot.x = _x;
		m_pivot.y = _x;
	}

	inline void SetDirty()
	{
		m_localTransform->dirty = true;
		m_worldTransform->dirty = true;
		for (auto& child : children)
		{
			if (auto c = child.lock())
				c->SetDirty();
		}
	}
};

