#pragma once
#include <Object/gameObject.h>
#include <Object/Camera.h>
#include <Helpers/StringHelper.h>
#include <unordered_set>

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

	void VisibleMemoryInfo();

	Camera* GetCamera()
	{
		return m_mainCamera;
	}

public:
	template<class TReturnType, typename... Args>
	TReturnType* NewObject(const std::wstring& NewobjectName, Args&&... args)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");
		std::unique_ptr<TReturnType> createdObj = std::make_unique<TReturnType>(std::forward<Args>(args)...);

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());
		m_nameToUUIDs[NewobjectName].insert(createdObj->GetUUID());
		TReturnType* rawPtr = createdObj.get();
		m_objects[createdObj->GetUUID()] = std::move(createdObj);

		return rawPtr;
	}

	bool RemoveObject(gameObject* targetObj);

	bool RemoveObjectByName(const std::wstring& objectName);

	bool RemoveAllObjectsByName(const std::wstring& name);

	template<class T>
	bool RemoveObjectByType()
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (std::dynamic_pointer_cast<T>(it->second))
			{
				const std::wstring& name = it->second->GetName();
				auto nameIt = m_nameToUUIDs.find(name);
				if (nameIt != m_nameToUUIDs.end()) {
					nameIt->second.erase(it->first);
					if (nameIt->second.empty())
						m_nameToUUIDs.erase(nameIt);
				}
				m_objects.erase(it);
				return true;
			}
		}
		return false;
	}

	template<class TReturnType, typename... Args>
	WeakObjectPtr<TReturnType> NewObjectByWeak(const std::wstring& NewobjectName, Args&&... args)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");
		TReturnType* createdObj = new TReturnType(std::forward<Args>(args)...);
		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());
		m_objects.emplace(createdObj->GetUUID(), createdObj);

		return WeakObjectPtr<TReturnType>(createdObj);
	}

	gameObject* Instantiate(gameObject* obj);

private:
	gameObject* m_sysinfoWidget;
	std::unordered_map<std::wstring, std::unique_ptr<gameObject>> m_objects;
	std::unordered_map<std::wstring, std::unordered_set<std::wstring>> m_nameToUUIDs;
};

