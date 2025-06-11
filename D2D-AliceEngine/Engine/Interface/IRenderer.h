#pragma once
#include "IComponent.h"

class IRenderer : public IComponent
{
public:
	void Initialize() override;
	void Update() override;
	virtual void Render() = 0;
	void Release() override;
};

