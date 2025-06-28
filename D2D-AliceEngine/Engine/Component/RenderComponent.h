#pragma once
#include "Component.h"

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();
public:
	void Initialize() override;
	void Update() override;
	void Release() override;

	virtual void Render() override;
};

