#pragma once
#include <Animation/AnimatorInstance.h>

class EnemyAnimatorInstance : public AnimatorInstance
{
public:
	EnemyAnimatorInstance();
	~EnemyAnimatorInstance();
	virtual void Initialize() override;
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override;
	virtual void Render() override;

public:
	virtual void SetAnimatorController(AnimatorController* controller) override;
	virtual void ChangeState(const std::string& stateName) override;
public:
	void OnStart() override;
	void OnExit();
};