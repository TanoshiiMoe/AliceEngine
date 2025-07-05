#pragma once
#include <Object/UObject.h>
#include <Component/Component.h>
#include <memory>
#include <Math/TMath.h>

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class TransformComponent;
class D2DRenderManager;
class gameObject : public UObject
{
public:
	gameObject() { Initialize(); }
	gameObject(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);	
	virtual ~gameObject();

	virtual void OnStart();
	virtual void OnEnd();
	/*
	* @ Component를 관리하는 로직입니다.
	*/

	std::vector<std::unique_ptr<Component>> m_Components;

	template<class T, typename... Args>
	T* AddComponent(Args&&... args)
	{	
		std::unique_ptr<T> createdComp = std::make_unique<T>(std::forward<Args>(args)...);
		createdComp->Initialize();
		createdComp->SetOwner(WeakFromThis<gameObject>());
		T* rawPtr = createdComp.get();
		m_Components.emplace_back(std::move(createdComp));

		return rawPtr;
	}

	template<class T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> container;
		for (auto component : m_Components)
		{
			if (auto castedComponent = dynamic_cast<T*>(component.get()))
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
			if (auto casted = dynamic_cast<T*>(comp.get()))
			{
				return casted; // raw pointer 반환
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
	WeakObjectPtr<T> AddComponentByWeak(Args&&... args)
	{
		std::unique_ptr<T> createdComp = std::make_unique<T>(std::forward<Args>(args)...);
		createdComp->Initialize();
		createdComp->SetOwner(WeakFromThis<gameObject>());
		T* rawPtr = createdComp.get();
		m_Components.emplace_back(std::move(createdComp));
		
		return WeakObjectPtr<T>(rawPtr);
	}

	template<class T>
	WeakObjectPtr<T> GetComponentByWeak()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (T* component = dynamic_cast<T*>((*it).get()))
			{
				return WeakObjectPtr<T>(component);
			}
		}
		return nullptr;
	}
	
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Initialize(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

	TransformComponent* transform() const { return m_transformComponent.lock(); }
	void AddChildObject(const gameObject* obj);

public:
	WeakObjectPtr<TransformComponent> m_transformComponent;
};