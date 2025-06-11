#include "pch.h"
#include "ComponentManager.h"

void ComponentManager::Initialize()
{
}

void ComponentManager::Uninitialize()
{
	DestroyAllComponent();
}

void ComponentManager::Update()
{
	for (auto& component : m_components)
	{
		component.second.get()->Update();
	}
}
