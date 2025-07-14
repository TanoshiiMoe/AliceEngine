#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>

RenderComponent::RenderComponent()
{
}

RenderComponent::~RenderComponent()
{
	RenderSystem::GetInstance().UnRegist(WeakFromThis<RenderComponent>());
}

void RenderComponent::Initialize()
{
	RenderSystem::GetInstance().Regist(WeakFromThis<RenderComponent>());
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
