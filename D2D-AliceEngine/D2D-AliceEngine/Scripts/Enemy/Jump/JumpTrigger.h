#pragma once
#include "Component/ScriptComponent.h"

class JumpTrigger : public ScriptComponent
{
public:
	gameObject* target;

	void Initialize() override;

	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void OnTriggerEnter2D(Collider* collider) override;
private:
	FVector2 offset;
};

