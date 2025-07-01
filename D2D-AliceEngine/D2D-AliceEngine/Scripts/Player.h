#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class Animator;
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

	Animator* m_animator;
	IdleState* idleState;
	AttackState* attackState;

	std::shared_ptr<SpriteSheet> Texture;
	std::shared_ptr<AnimationClip> idle;
	std::shared_ptr<AnimationClip> attack;
};

