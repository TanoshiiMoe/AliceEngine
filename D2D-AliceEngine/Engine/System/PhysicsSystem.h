#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <Object/UObject.h>
#include <Component/Rigidbody2D.h>
/*
* @briefs : 전역으로 모든 RigidBody2D를 계산하는 시스템입니다.
* @details :
*/

class RigidBody2D;
class PhysicsSystem : public ITickable, public UObject, public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

public:
	void Regist(const WeakObjectPtr<RigidBody2D>& _component);
	void UnRegist(WeakObjectPtr<RigidBody2D>&& _component);
	void UnRegistAll();
	virtual void Update(const float& deltaSeconds) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	std::unordered_set<Rigidbody2D*> collidedBodies;
protected:
	std::vector<WeakObjectPtr<RigidBody2D>> m_rigidBodies;
};