#include "PlayerCrashColl.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "Component/BoxComponent.h"
#include "Manager/UpdateTaskManager.h"
#include "System/ScriptSystem.h"
#include "Scripts/Player/PlayerManager.h"

void PlayerCrashColl::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void PlayerCrashColl::OnStart()
{
	crashColl = GetWorld()->NewObject<gameObject>(L"PlayerCrashColl");
	crashColl->SetTag(L"Player");

	// 차량충돌 콜라이더 설정
	Collider* cc = crashColl->AddComponent<Collider>();
	cc->SetBoxSize(FVector2(150, 10));
	cc->SetLayer(5);

	PlayerManager::instance->SetCrashColl(cc);

	target = PlayerManager::instance->GetOwner();
}

void PlayerCrashColl::Update(const float& deltaTime)
{
	crashColl->transform()->SetPosition(target->transform()->GetPosition() + FVector2(0, -50));
}
