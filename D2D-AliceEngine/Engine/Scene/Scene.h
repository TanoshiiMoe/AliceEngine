#pragma once
#include <Object/gameObject.h>
#include <Object/Camera.h>
#include <Helpers/StringHelper.h>

class Scene : public UObject
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
	template<class TReturnType, typename... Args>
	TReturnType* NewObject(const std::wstring& NewobjectName, Args&&... args)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<TReturnType> createdObj = std::make_shared<TReturnType>(std::forward<Args>(args)...);

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

	bool RemoveObjectByName(const std::wstring& objectName)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			// 키 값이 objectName과 같으면 삭제 (UUID로 저장되어 있다면 GetName() 비교)
			if (it->second && it->second->GetName() == objectName)
			{
				it->second.reset();
				m_objects.erase(it);
				return true;
			}
		}
		return false;
	}

	template<class T>
	bool RemoveObjectByType()
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (it->second && it->second == typeid(T))
			{
				it->second.reset();
				m_objects.erase(it);
				return true;
			}
		}
		return false;
	}


private:
	gameObject* m_sysinfoWidget;
	std::unordered_map<std::wstring, std::shared_ptr<gameObject>> m_objects;
};

