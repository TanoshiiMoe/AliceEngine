#pragma once
#include "Component.h"
#include <System/RenderSystem.h>

class RenderComponent : public Component
{
public:
	RenderComponent() { RenderSystem::Get().Regist(this->weak_from_this()); }
	~RenderComponent() { RenderSystem::Get().UnRegist(this->weak_from_this()); }
public:
	void Initialize() override;
	void Update() override;
	void Release() override;

	virtual void Render() = 0;
};

