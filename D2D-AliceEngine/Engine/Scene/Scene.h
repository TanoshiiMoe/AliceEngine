#pragma once
#include "pch.h"
#include "Object/gameObject.h"
#include "Object/Object.h"
#include "Camera.h"

class Scene : public Object
{
public:
	Scene();
	virtual ~Scene();

	Camera* m_mainCamera; // Main Camera

	void Initialize() override;
	void Release() override;
	void Update() override;

	Camera* GetCamera()
	{
		return m_mainCamera;
	}

public:
	template<class TReturnType>
	TReturnType* NewObject(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<TReturnType> createdObj = std::make_shared<TReturnType>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = m_objects.emplace(createdObj->GetUUID(), createdObj);
		auto iter = result.first;

		return dynamic_cast<TReturnType*>(createdObj.get());
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<Object>> m_objects;
};

