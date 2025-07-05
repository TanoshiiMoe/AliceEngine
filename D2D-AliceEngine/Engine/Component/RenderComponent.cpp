#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>

RenderComponent::RenderComponent()
{
	RenderSystem::GetInstance().Regist(WeakFromThis<Component>());
}

RenderComponent::~RenderComponent()
{
	RenderSystem::GetInstance().UnRegist(WeakFromThis<Component>());
}

void RenderComponent::Initialize()
{
	D2DRenderManager::GetInstance().AddRenderer(WeakFromThis<RenderComponent>());
}

void RenderComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void RenderComponent::Release()
{
}

void RenderComponent::Render()
{
	__super::Render();
}
