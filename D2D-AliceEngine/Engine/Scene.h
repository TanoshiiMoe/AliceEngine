#pragma once
#include "pch.h"
#include "Object/Object.h"

class Scene : public std::enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize();
	virtual void Release();
	virtual void Update();
	virtual void Render();

public:
	template<class TReturnType>
	std::weak_ptr<TReturnType> NewObject(const std::wstring& NewobjectName)
	{
		if (std::is_base_of_v<IObject, TReturnType>() == false)
		{
			//std::cout << "NewObject must be derived from IObject";
			return nullptr;
		}
		std::shared_ptr<IObject> createdObj = std::make_shared<TReturnType>();
		createdObj->m_objectName = NewobjectName;
		createdObj->m_uuid = NewobjectName + StringHelper::MakeUniqueName();
		auto result = m_objects.emplace(createdObj->m_uuid, createdObj);
		auto iter = result.first;
		return std::dynamic_pointer_cast<TReturnType>(iter->second);
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<IObject>> m_objects;
};

