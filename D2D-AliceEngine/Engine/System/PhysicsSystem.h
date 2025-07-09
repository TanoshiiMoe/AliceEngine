#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <Object/UObject.h>
#include <Experimental/Physics/AABB.h>
/*
* @briefs : 전역으로 모든 Collider의 충돌을 감지하는 시스템입니다.
* @details :
*	업데이트 함수를 쓰기 위해 ITickable을 상속받고
*	오브젝트 핸들을 사용하기 위해 UObject를 상속받습니다.
*	현재 등록되어 있는 모든 Collider의 충돌 검사를 매 Update마다 실행합니다.
*/

class Collider;
class PhysicsSystem : public ITickable, public UObject, public Singleton<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();

public:
	void Regist(const WeakObjectPtr<Collider>& _component);
	void UnRegist(WeakObjectPtr<Collider>&& _component);
	void UnRegistAll();
	virtual void Update(const float& deltaSeconds) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

protected:
	std::vector<WeakObjectPtr<Collider>> m_AABBs;
};

