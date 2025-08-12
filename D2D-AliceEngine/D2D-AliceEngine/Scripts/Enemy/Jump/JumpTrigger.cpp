#include "JumpTrigger.h"
#include "System/ScriptSystem.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "GameManager/GamePlayManager.h"
#include <Scripts/Player/PlayerManager.h>
#include "Manager/UpdateTaskManager.h"

void JumpTrigger::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void JumpTrigger::OnStart()
{
	/*if (target)
		offset = owner->transform()->GetPosition() - target->transform()->GetPosition();
	else
		OutputDebugStringW(L"점프트리거에 타겟이 없음!!!!\n");*/
}

void JumpTrigger::Update(const float& deltaSeconds)
{
	if (target.IsValid()) {
		FVector2 targetPos = target->transform()->GetPosition();
		targetPos += offset;

		owner->transform()->SetPosition(targetPos);
	}
}

void JumpTrigger::OnTriggerEnter2D(Collider* collider)
{
	if (collider->GetOwner()->GetName() == L"Player") {
		GamePlayManager::GetInstance().GetPlayer()->GetComponent<PlayerManager>()->Jump();
	}
}
