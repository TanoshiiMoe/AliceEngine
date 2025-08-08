#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimationController.h"
class Car : public ScriptComponent
{
public:
	Car() {}
	virtual ~Car() {}

	void Initialize() override;
	void OnStart() override;
protected:
	AnimatorController animController;
};

