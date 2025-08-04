#pragma once
#include <Core/Singleton.h>
#include <memory>
#include <vector>
#include <functional>
#include <Scene/Scene.h>
#include <unordered_map>
#include <Core/Tickable.h>
#include <Define/Define.h>

#define REGISTER_UPDATE_TASK_IN_SCRIPT(FunctionName, TickGroup) \
	UpdateTaskManager::GetInstance().Enque( \
		WeakFromThis<ITickable>(), \
		TickGroup, \
		[weak = WeakFromThis<ITickable>()](const float& dt) \
		{ \
			if (auto sp = weak.lock()) \
			{ \
				if (auto script = dynamic_cast<ScriptComponent*>(sp)) \
				{ \
					script->FunctionName(dt); \
				} \
			} \
		} \
	)

#define REGISTER_TICK_TASK(FunctionName, TickGroup) \
	UpdateTaskManager::GetInstance().Enque( \
		WeakFromThis<ITickable>(), \
		TickGroup, \
		[weak = WeakFromThis<ITickable>()](const float& dt) \
		{ \
			if (auto sp = weak.lock()) \
			{ \
				sp->FunctionName(dt); \
			} \
		} \
	)

/*
* @brief : Update()�ȿ����� ������ ���ϱ� ���� �Ŵ����Դϴ�
* @details :
*	Update �Լ��� �ܰ踦 ������ ��ϵ� �������(Ȥ�� �����Ͽ�) ������ �Լ� �����͸� �����մϴ�.
*	����� ���θ� �𸮾� ��Ÿ�Ϸ� ��������ϴ�.
*/

// Unreal �� Unity �ֿ� Tick/Update ��
// TG_PrePhysics       : EarlyUpdate         // (AI/�Է�, Unity�� EarlyUpdate)
// TG_StartPhysics     : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate �غ�)
// TG_DuringPhysics    : FixedUpdate         // (���� ����, Unity�� FixedUpdate)
// TG_EndPhysics       : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate ��ó��)
// TG_PostPhysics      : Update/LateUpdate   // (���ӷ���, Unity�� Update/LateUpdate)
// TG_PostUpdateWork   : PreRender/Render    // (ī�޶�, ������, Unity�� PreRender/Render)
// TG_LastDemotable    : (Ư�� ��Ȳ��, �𸮾��� �ܰ�)
// TG_NewlySpawned     : (�� ��ü ó����, �𸮾��� �ܰ�)

/** Update() �Լ��� �����ؾ߸� �ϴ� ��ϵ��Դϴ� */
struct FTickContext
{
	/** Update �ǰ� �ִ� ���� ������ ���� �� */
	Scene* World;
	// ���� Update���� Group�� �������� �˷��ִ� ����
	Define::ETickingGroup TickGroup;
	/** Update�� ���� deltatime */
	float DeltaSeconds;
	/** FixedUpdate�� ���� deltatime */
	float FixedDeltaSeconds;
	/** true�� Update�� �α� ó���� */
	bool bLogTick;

	FTickContext(float InDeltaSeconds = 0.0f, float InFixedDeltaSeconds = 0.0f, Define::ETickingGroup InTickGroup = Define::ETickingGroup::TG_PrePhysics)
		: World(nullptr)
		, DeltaSeconds(InDeltaSeconds)
		, FixedDeltaSeconds(InFixedDeltaSeconds)
		, TickGroup(InTickGroup)
		, bLogTick(false)
	{
	}
	FTickContext(const FTickContext& In) = default;
	FTickContext& operator=(const FTickContext& In) = default;
};

struct UpdateWrapper
{
	WeakObjectPtr<ITickable> Target;
	std::function<void(const float&)> TickFunc;

	UpdateWrapper(WeakObjectPtr<ITickable> _Target, std::function<void(const float&)> _TickFunc)
		: Target(_Target), TickFunc(_TickFunc)
	{
	}
};

class UpdateTaskManager : public Singleton<UpdateTaskManager>
{
private:
	FTickContext Context;
public: 
	std::unordered_map<Define::ETickingGroup, std::vector<UpdateWrapper>> m_TickLists;

	void Enque(WeakObjectPtr<ITickable> InTarget, Define::ETickingGroup InGroup, std::function<void(const float&)> TickFunc);

	void StartFrame();
	void EndFrame();
	void SetWorld();
	void ClearWorld();

	void TickAll();
};

