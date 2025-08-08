#pragma once
#include "Component/ScriptComponent.h"
#include <Manager/TimerManager.h>
class Car : public ScriptComponent
{
public:
	Car() {}
	virtual ~Car() {}

	void Initialize() override;
	void OnStart() override;

	FTimerHandle timer;
};

