#pragma once
#include <Core/Singleton.h>
#include <memory>
#include <vector>
#include <functional>
#include <Scene/Scene.h>

/*
* @brief : Update()안에서의 순서를 정하기 위한 매니저입니다
* @details :
*	Update 함수의 단계를 나누어 등록된 순서대로(혹은 정렬하여) 각각의 함수 포인터를 실행합니다.
*	여기는 내부를 언리얼 스타일로 만들었습니다.
*/

// Unreal ↔ Unity 주요 Tick/Update 비교
// TG_PrePhysics       : EarlyUpdate         // (AI/입력, Unity의 EarlyUpdate)
// TG_StartPhysics     : FixedUpdate 시작 전 // (Unity에 직접 대응 없음, 보통 FixedUpdate 준비)
// TG_DuringPhysics    : FixedUpdate         // (물리 연산, Unity의 FixedUpdate)
// TG_EndPhysics       : FixedUpdate 종료 후 // (Unity에 직접 대응 없음, 보통 FixedUpdate 후처리)
// TG_PostPhysics      : Update/LateUpdate   // (게임로직, Unity의 Update/LateUpdate)
// TG_PostUpdateWork   : PreRender/Render    // (카메라, 렌더링, Unity의 PreRender/Render)
// TG_LastDemotable    : (특수 상황용, 언리얼만의 단계)
// TG_NewlySpawned     : (새 객체 처리용, 언리얼만의 단계)

/** Update() 함수가 실행해야만 하는 목록들입니다 */
struct FTickContext
{
	/** Update 되고 있는 동안 가지고 있을 씬 */
	Scene* World;
	Define::ETickingGroup TickGroup;
	/** Update에 대한 deltatime */
	float DeltaSeconds;
	/** true면 Update를 로그 처리함 */
	bool bLogTick;

	FTickContext(float InDeltaSeconds = 0.0f, Define::ETickingGroup InTickGroup = Define::ETickingGroup::TG_PrePhysics)
		: World(nullptr)
		, DeltaSeconds(InDeltaSeconds)
		, TickGroup(InTickGroup)
		, bLogTick(false)
	{
	}
	FTickContext(const FTickContext& In) = default;
	FTickContext& operator=(const FTickContext& In) = default;
};

class UpdateTaskManager : public Singleton<UpdateTaskManager>
{
public: 
	std::function<void()> AllTickFunctions;
	FTickContext Context;

	void StartFrame(Scene* InWorld, float InDeltaSeconds)
	{
		Context.TickGroup = Define::ETickingGroup::TG_PrePhysics;
		Context.DeltaSeconds = InDeltaSeconds;
		Context.World = InWorld;
	}
};

