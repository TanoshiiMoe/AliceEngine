#pragma once
#include <Component/ScriptComponent.h>
#include <Animation/AnimationController.h>

class gameObject;
class AnimatorInstance;
class Player : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	void Input();

	gameObject* m_owner;
	int jumpCount = 0;
	int maxJumpCount = 2;
	int prevGroundState = 0;

	float walkSpeed = 120;

	bool bMoveRigidBody = true;

	AnimatorInstance* animInstance;
	AnimatorController animController;
};

