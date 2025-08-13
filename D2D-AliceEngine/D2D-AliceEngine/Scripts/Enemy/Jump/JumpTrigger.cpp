#include "JumpTrigger.h"
#include "System/ScriptSystem.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "GameManager/GamePlayManager.h"
#include <Scripts/Player/PlayerManager.h>
#include "Manager/UpdateTaskManager.h"
#include "Component/BoxComponent.h"

void JumpTrigger::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);

	Collider* col = owner->AddComponent<Collider>();
	col->SetBoxSize(FVector2(10.0f, 50.0f));
	col->SetLayer(5);
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
		targetPos.x += offset.x;

		owner->transform()->SetPosition(targetPos);
	}
}

void JumpTrigger::OnTriggerEnter2D(Collider* collider)
{
	if (collider->GetOwner()->GetTag() == L"Player") {
		GamePlayManager::GetInstance().GetPlayer()->GetComponent<PlayerManager>()->Jump();
	}
}

void JumpTrigger::SetOffSet(FVector2 _offset)
{
	offset = _offset;
	owner->GetComponent<Collider>()->boxComponent->SetRelativePosition(FVector2(0.0f, _offset.y));
}

void JumpTrigger::SetBoxSize(FVector2 _size)
{
	owner->GetComponent<Collider>()->boxComponent->SetSize(_size);
}
