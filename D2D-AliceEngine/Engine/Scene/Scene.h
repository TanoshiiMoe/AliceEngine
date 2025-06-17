#pragma once
#include "pch.h"
#include "Object/gameObject.h"
#include "Object/Object.h"
#include <Object/Camera.h>

class Scene : public Object
{
public:
	Scene();
	virtual ~Scene();

	Camera* m_mainCamera; // Main Camera

	void Initialize() override;
	void Release() override;
	void Update() override;

	virtual void OnEnter();
	virtual void OnExit();

	Camera* GetCamera()
	{
		return m_mainCamera;
	}

public:
	template<class TReturnType>
	TReturnType* AddObject(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<TReturnType> createdObj = std::make_shared<TReturnType>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = m_objects.emplace(createdObj->GetUUID(), createdObj);
		auto iter = result.first;

		return dynamic_cast<TReturnType*>(createdObj.get());
	}

	template<class T>
	void RemoveObject(std::weak_ptr<T>& targetObj)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (*it == targetObj)
			{
				it->second.reset();
				m_objects.erase(it);
				return true;
			}
		}
		return false;
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<gameObject>> m_objects;
};

