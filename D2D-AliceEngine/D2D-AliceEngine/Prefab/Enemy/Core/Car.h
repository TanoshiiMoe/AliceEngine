#pragma once
#include "Component/ScriptComponent.h"
class Car : public ScriptComponent
{
public:
	Car() {}
	virtual ~Car() {}

	void Initialize() override;
	void OnStart() override;
};

