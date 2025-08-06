#pragma once
#include "Component/ScriptComponent.h"
class Car : public ScriptComponent
{
public:
	float moveSpeed = 0.0f;

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void Move();
	void Stop();
private:
	float isMoving = true;
};

