#include "SpawnCollider.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "Component/TransformComponent.h"
#include "Manager/SceneManager.h"
#include <System/ScriptSystem.h>
#include "Manager/UpdateTaskManager.h"

void SpawnCollider::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void SpawnCollider::OnStart()
{
	co = owner->AddComponent<Collider>();
	owner->transform()->SetPivot(0.5f, 0.5f);
	//owner->SetParent(SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player")->transform());

	co->SetBoxSize(FVector2(500.0f, 500.0f));
	co->SetLayer(0);

	player = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");
}

void SpawnCollider::Update(const float& deltaSeconds)
{
	owner->transform()->SetPosition(player->transform()->GetPosition());
}

void SpawnCollider::OnTriggerEnter2D(Collider* collider)
{
	std::wstring message = collider->GetOwner()->GetName() + L" : 스포너 콜라이더 Enter!!\n";
	OutputDebugStringW(message.c_str());
}
