#pragma once
#include "pch.h"
#include "Object/Object.h"
#include "Interface/IObject.h"

class Scene : public IObject
{
public:
	Scene();
	virtual ~Scene();

	void Initialize() override;
	void Release() override;
	void Update() override;

public:
	template<class TReturnType>
	std::weak_ptr<TReturnType> NewObject(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<IObject> createdObj = std::make_shared<TReturnType>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = m_objects.emplace(createdObj->GetUUID(), createdObj);
		auto iter = result.first;

		return std::dynamic_pointer_cast<TReturnType>(iter->second);
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<IObject>> m_objects;
};

