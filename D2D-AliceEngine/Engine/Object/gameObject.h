#pragma once
#include "../pch.h"
#include <Math/Transform.h>
#include <Component/SpriteRenderer.h>
#include <System/RenderSystem.h>
#include <Object/UObject.h>
#include <Component/TransformComponent.h>
#include <Manager/D2DRenderManager.h>

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class D2DRenderManager;
class gameObject : public UObject, public std::enable_shared_from_this<gameObject>
{
public:
	gameObject() { Initialize(); }
	gameObject(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);
	
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
		m_Components.clear();
	}

	/*
	* @ Component를 관리하는 로직입니다.
	*/

	std::vector<std::shared_ptr<Component>> m_Components;

	template<class T, typename... Args>
	T* AddComponent(Args&&... args)
	{	
		std::shared_ptr<T> createdComp = std::make_shared<T>(std::forward<Args>(args)...);

		createdComp->Initialize();
		createdComp->SetOwner(this->weak_from_this());
		createdComp->SetOwnerName(GetName());
		createdComp->SetUUID(StringHelper::MakeUniqueName());

		if (auto renderComp = std::dynamic_pointer_cast<RenderComponent>(createdComp))
		{
			std::weak_ptr<RenderComponent> weakRender = renderComp;
			D2DRenderManager::GetInstance().AddRenderer(weakRender);
			weakRender.lock()->m_pTransform = m_transformComponent.lock()->m_worldTransform;
			weakRender.lock()->m_pivot = &m_transformComponent.lock()->m_pivot;
		}

		m_Components.emplace_back(createdComp);

		return createdComp.get();
	}

	template<class T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> container;
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
	T* GetComponent()
	{
		for (auto& comp : m_Components)
		{
			// shared_ptr<Component> → shared_ptr<T>로 캐스팅
			if (auto casted = std::dynamic_pointer_cast<T>(comp))
			{
				return casted.get(); // raw pointer 반환
			}
		}
		return nullptr;
	}


	template<class T>
	bool RemoveComponent(T* targetComponent)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if ((*it).get() == targetComponent)
			{
				(*it)->OnDestroy();
				m_Components.erase(it);
				return true;
			}
		}
		return false;
	}

	template<class T, typename... Args>
	std::weak_ptr<T> AddComponentByWeak(Args&&... args)
	{
		std::shared_ptr<T> createdComp = std::make_shared<T>(std::forward<Args>(args)...);

		createdComp->Initialize();
		createdComp->SetOwner(this->weak_from_this());
		createdComp->SetOwnerName(GetName());
		createdComp->SetUUID(StringHelper::MakeUniqueName());

		if (auto renderComp = std::dynamic_pointer_cast<RenderComponent>(createdComp))
		{
			std::weak_ptr<RenderComponent> weakRender = renderComp;
			D2DRenderManager::GetInstance().AddRenderer(weakRender);
			weakRender.lock()->m_pTransform = m_transformComponent.lock()->m_worldTransform;
			weakRender.lock()->m_pivot = &m_transformComponent.lock()->m_pivot;
		}

		m_Components.emplace_back(createdComp);

		return std::dynamic_pointer_cast<T>(createdComp);
	}

	template<class T>
	std::weak_ptr<T> GetComponentByWeak()
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
	
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Initialize(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

	TransformComponent* transform() const { return m_transformComponent.lock().get(); }

	void AddChildObject(const gameObject* obj)
	{
		this->transform()->AddChildObject(obj->m_transformComponent);
	}

public:
	std::weak_ptr<TransformComponent> m_transformComponent;
};