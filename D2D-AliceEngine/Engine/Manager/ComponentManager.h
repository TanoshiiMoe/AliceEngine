#pragma once
#include "Interface/IComponent.h"
#include "StringHelper.h"
#include "Singleton.h"

class ComponentManager : public Singleton<ComponentManager>
{
public:
	void Initialize();
	void Uninitialize();
	void Update();

public:
	template<class TReturnType>
	std::weak_ptr<TReturnType> CreateDefaultSubobject(const std::wstring& SubobjectName, const std::wstring& ownerName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<IComponent> createdComp = std::make_shared<TReturnType>();

		createdComp->SetName(SubobjectName);
		createdComp->SetOwnerName(ownerName);
		createdComp->SetUUID(SubobjectName + StringHelper::MakeUniqueName());

		auto result = m_components.emplace(createdComp->GetUUID(), createdComp);
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
#define GetD2DRenderManager() D2DRenderManager::Get()
#define CreateDefaultSubobject(Type, SubobjectName, ownerName) ComponentManager::Get().CreateDefaultSubobject<Type>(SubobjectName, ownerName)