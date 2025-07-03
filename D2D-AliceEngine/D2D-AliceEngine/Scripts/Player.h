#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class PlayerAnimatorInstance;
class IdleState;
class AttackState;
struct SpriteSheet;
struct AnimationClip;
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

	PlayerAnimatorInstance* animInstance;
	IdleState* idleState;
	AttackState* attackState;
};

