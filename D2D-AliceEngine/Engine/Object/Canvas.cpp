#include "pch.h"
#include "Canvas.h"

Canvas::~Canvas()
{
	for (auto& component : m_components)
	{
		if (component)
		{
			component->OnDestroy();
			delete component;
		}
	}
	m_components.clear();
	m_transformComponent.reset();
}

void Canvas::Initialize()
{
}

void Canvas::Update()
{
	for (auto& comp : m_components)
	{
		comp->Update();
	}
}

void Canvas::Release()
{
}

void Canvas::AddChildObject(const gameObject* obj)
{
}
