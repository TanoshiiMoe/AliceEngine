#pragma once
#include "Component/ScriptComponent.h"

class BikeMovementScript;

class PlayerMovement : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void SetBikeMovment(BikeMovementScript* _bikeMovement) { bMovement = _bikeMovement; }
	void SetInitSpeed(float _speed) { initSpeed = _speed; }
	float GetInitSpeed() { return initSpeed; }

private:
	BikeMovementScript* bMovement;
	float initSpeed;
	FVector2 moveDir = { 0.0f, 0.0f };
	float dt;

	void Input();
};

