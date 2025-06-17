#pragma once
#include "../pch.h"
#include <Math/Transform.h>
#include <Component/BitmapRenderer.h>
#include "../System/RenderSystem.h"
#include "Object.h"
#include <Component/TransformComponent.h>

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class gameObject : public Object, public std::enable_shared_from_this<gameObject>
{
public:
	gameObject(){}
	gameObject(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
	{
		Initialize(path, position, rotation, scale, pivot);
	}
	virtual ~gameObject()
	{
		for (std::weak_ptr<Component>&& comp : m_Components) 
		{
			comp.lock()->OnDestroy();
		}
		m_Components.clear();
	}

	virtual void OnStart()
	{
		for (std::weak_ptr<Component>&& comp : m_Components)
		{
			comp.lock()->OnStart();
		}
	}

	virtual void OnEnd()
	{
		for (std::weak_ptr<Component>&& comp : m_Components)
		{
			comp.lock()->OnEnd();
		}
	}

	/*
	* @ Component를 관리하는 로직입니다.
	*/

	std::vector<std::shared_ptr<Component>> m_Components;

	template<class T, typename... Args>
	std::weak_ptr<T> AddComponent(Args&&... args)
	{	
		std::shared_ptr<T> createdComp = std::make_shared<T>(std::forward<Args>(args)...);

		createdComp->Initialize();
		createdComp->SetOwner(this->weak_from_this());
		createdComp->SetOwnerName(GetName());
		createdComp->SetUUID(StringHelper::MakeUniqueName());

		m_Components.emplace_back(createdComp);

		return std::dynamic_pointer_cast<T>(createdComp);
	}

	template<class T>
	std::vector<std::weak_ptr<T>> GetComponents()
	{
		std::vector<std::weak_ptr<T>> container;
		for (auto component : m_Components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component.get()))
			{
				container.push_back(castedComponent);
			}
		}
		return container;
	}

	template<class T>
	std::weak_ptr<T> GetComponent()
	{
		return std::dynamic_pointer_cast<T>(FindComponent<T>());
	}

	template<class T>
	std::weak_ptr<T> FindComponent()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).get())
			{
				if (auto component = std::dynamic_pointer_cast<T>((*it).get()))
				{
					return component;
				}
			}
		}
		return std::weak_ptr<T>();
	}

	template<class T>
	void RemoveComponent(std::weak_ptr<T>& targetComponent)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (*it == targetComponent)
			{
				(*it)->OnDestroy();
				m_Components.erase(it);
				return true;
			}
		}
		return false;
	}
	
	void Initialize() override;
	void Update() override;
	void Release() override;
	virtual void Initialize(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f));
	void LoadData(const std::wstring& path);

public:
	std::weak_ptr<SpriteRenderer> m_bitmapRenderer;
	std::weak_ptr<TransformComponent> m_transformComponent;

	inline std::weak_ptr<SpriteRenderer> GetRenderer()
	{
		return m_bitmapRenderer;
	}

};

#define transform m_transformComponent.lock()