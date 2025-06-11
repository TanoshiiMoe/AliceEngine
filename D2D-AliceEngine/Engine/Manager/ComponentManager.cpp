#include "pch.h"
#include "ComponentManager.h"

void ComponentManager::Initialize()
{
}

void ComponentManager::Uninitialize()
{
	DestroyAllComponent();
}

void ComponentManager::Render()
{
	for (auto& component : m_components)
	{
		component.second.get()->Render();
	}
}

void ComponentManager::Update()
{
	for (auto& component : m_components)
	{
		component.second.get()->Update();
	}
}
