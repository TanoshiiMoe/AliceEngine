#pragma once
#include "Component/ScriptComponent.h"
class PlayerCrashColl : public ScriptComponent
{
public:
	void Initialize() override;

	void OnStart() override;
	void Update(const float& deltaTime) override;

private:
	gameObject* target;
	gameObject* crashColl;
};

