#include "pch.h"
#include "RenderComponent.h"

void RenderComponent::Initialize()
{
	RenderSystem::GetInstance().Regist(this->weak_from_this());
}

void RenderComponent::Update()
{
}

void RenderComponent::Release()
{
}
