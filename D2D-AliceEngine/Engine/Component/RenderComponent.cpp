#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>

RenderComponent::RenderComponent()
{
}

RenderComponent::~RenderComponent()
{
	RenderSystem::GetInstance().UnRegist(this->weak_from_this());
}

void RenderComponent::Initialize()
{
	RenderSystem::GetInstance().Regist(this->weak_from_this());
	D2DRenderManager::GetInstance().AddRenderer(this->weak_from_this());
}

void RenderComponent::Update()
{
}

void RenderComponent::Release()
{
}

void RenderComponent::Render()
{
	__super::Render();
}
