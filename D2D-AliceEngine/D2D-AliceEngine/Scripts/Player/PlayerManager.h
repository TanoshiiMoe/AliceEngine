#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimatorInstance.h"
#include "Animation/AnimationController.h"

class PlayerManager : public  ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

private:
	AnimatorInstance* animInstance = nullptr;
	AnimatorController animController;

	void Input();

	float playerTimeScale = 1.0f;
};

