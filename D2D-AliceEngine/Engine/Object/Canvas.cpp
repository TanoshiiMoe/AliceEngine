#include "pch.h"
#include "Canvas.h"
#include <Component/TransformComponent.h>

Canvas::Canvas()
{
	Initialize();
}

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
	/*TransformComponent* tc = new TransformComponent();
	tc->SetCanvas(WeakFromThis<Canvas>());
	tc->Initialize();
	m_transformComponent = WeakObjectPtr<TransformComponent>(tc);*/
	m_transformComponent = new TransformComponent();
	m_transformComponent->SetCanvas(WeakFromThis<Canvas>());
	m_transformComponent->Initialize();
}

void Canvas::Update()
{
	//for (auto& comp : m_components)
	//{
	//	comp->Update();
	//}
}

void Canvas::Release()
{
}

void Canvas::AddToChildUI(const UIComponent* ucmp)
{
	m_transformComponent->AddChildObject(ucmp->GetUITransform());
}