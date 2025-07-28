#pragma once
#include <Object/UObject.h>
#include <Component/UIComponent.h>
#include <memory>
#include <Math/TMath.h>
#include <Core/ObjectHandler.h>
#include <vector>

class TransformComponent;
class D2DRenderManager;
class Canvas : public UObject
{
public:
	Canvas();
	virtual ~Canvas();

public:
	std::vector<UIComponent*> m_components;

	template<class T, typename... Args>
	T* AddUI(Args&&... args)
	{
		T* createdComp = new T(std::forward<Args>(args)...);
		createdComp->SetCanvas(WeakFromThis<Canvas>());
		createdComp->Initialize();
		m_components.emplace_back(createdComp);

		return createdComp;
	}

	template<class T>
	std::vector<T*> GetUIs()
	{
		std::vector<T*> container;
		for (auto component : m_components)
		{
			if (WeakObjectPtr<T> castedComponent = dynamic_cast<T*>(component))
			{
				container.push_back(castedComponent.Get());
			}
		}
		return container;
	}

	template<class T>
	T* GetUI()
	{
		for (auto& comp : m_components)
		{
			// shared_ptr<Component> → shared_ptr<T>로 캐스팅
			if (auto casted = dynamic_cast<T*>(comp))
			{
				return casted; // raw pointer 반환
			}
		}
		return nullptr;
	}

	template<class T>
	bool RemoveUI(T* targetComponent)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it) == targetComponent)
			{
				(*it)->OnDestroy();
				m_components.erase(it);
				delete targetComponent;
				return true;
			}
		}
		return false;
	}

	//virtual void OnStart();
	//virtual void OnEnd();
	/*
	* @ Component를 관리하는 로직입니다.
	*/

	void Initialize() override;
	void Update() override;
	void Release() override;

	TransformComponent* transform() const { return m_transformComponent.lock(); }
	void AddChildUI(const UIComponent* ucmp);

	WeakObjectPtr<TransformComponent> m_transformComponent;
};