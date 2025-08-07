#pragma once
#include "Component/ScriptComponent.h"
#include "Core/Delegate.h"
class EnemyManager : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;

	void Update(const float& deltaSeconds) override;

public:
	float moveSpeed = 50.0f;
	float isMoving = true;

	void Move(const float& dt);
	void MoveUp();
	void MoveDown();
	void Die();

	void AddDieAction(ObjectHandle _handle, std::function<void()> _func);
	void RemoveDieAction(ObjectHandle _handle);
	void ClearDieAction();
private:
	MultiDelegate<> dieActions;
};

