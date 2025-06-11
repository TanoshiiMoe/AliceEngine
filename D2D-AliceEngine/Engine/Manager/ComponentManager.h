#pragma once
#include "../Interface/IComponent.h"
#include "../StringHelper.h"
#include "../Singleton.h"

class ComponentManager : public Singleton<ComponentManager>
{
public:
	void Initialize();
	void Uninitialize();
	void Render();
	void Update();

public:
	template<class TReturnType>
	std::weak_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName)
	{
		//if (std::is_base_of_v<IComponent, TReturnType>() == false)
		//{
		//	std::cout << "CreateDefaultSubobject must be derived from IComponent";
		//	return nullptr;
		//}
		std::shared_ptr<IComponent> createdComp = std::make_shared<TReturnType>();
		createdComp->m_objectName = SubobjectName;
		createdComp->m_uuid = SubobjectName + StringHelper::MakeUniqueName();
		auto result = m_components.emplace(createdComp->m_uuid, createdComp);
		auto iter = result.first;
		return std::dynamic_pointer_cast<TReturnType>(iter->second);
	}

	void DestroyAllComponent()
	{
		m_components.clear();
	}

	void DestroyComponent(const std::wstring& name)
	{
		if (name.empty() == false)
			m_components.erase(name);
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<IComponent>> m_components;
};

#define CreateDefaultSubobject(Type, SubobjectName) ComponentManager::GetInstance()->CreateDefaultSubobject<Type>(SubobjectName)