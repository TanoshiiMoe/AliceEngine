#pragma once
#include "Component.h"

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	virtual void Render() override;

	int m_layer = -999;
};

