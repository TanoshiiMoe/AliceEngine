#include "pch.h"
#include "RenderComponent.h"

void RenderComponent::Initialize()
{
	RenderSystem::Get().Regist(this->weak_from_this());
}

void RenderComponent::Update()
{
}

void RenderComponent::Release()
{
}
