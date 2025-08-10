#pragma once
#include "Component/ScriptComponent.h"
#include "Animation/AnimatorInstance.h"
#include "Animation/AnimationController.h"

class PlayerManager : public  ScriptComponent
{
public:
	static PlayerManager* instance;

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void SetZClamp(float _min, float _max);
private:
	AnimatorInstance* animInstance = nullptr;
	AnimatorController animController;

	void Input();


	float minZ = -230.0f;
	float maxZ = 410.0f;

	float playerTimeScale = 1.0f;
};

