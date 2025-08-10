#include "PlayerManager.h"
#include "System/ScriptSystem.h"
#include "Component/SkewTransform.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimationController.h"
#include "Component/Collider.h"
#include "Component/BoxComponent.h"
#include <Core/Input.h>
#include "Effect/Prism.h"
#include "Manager/TimerManager.h"
#include <Component/InputComponent.h>
#include "Manager/UpdateTaskManager.h"
#include "../Bike/BikeMovementScript.h"

PlayerManager* PlayerManager::instance = nullptr;

void PlayerManager::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void PlayerManager::OnStart()
{
	__super::Initialize();

	// instance 설정
	if (instance == nullptr)
		instance = this;
	else
		SceneManager::GetInstance().GetWorld()->RemoveObject(this->owner.lock());

	// SkewTransform 설정
	if (auto st = owner->GetComponent<SkewTransform>())
		st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");

	// SRT 설정
	owner->transform()->SetPosition(0, 0);
	owner->transform()->SetRotation(0);
	owner->transform()->SetScale(0.7f, 0.7f);
	owner->transform()->SetPivot(0.5f);

	// 애니메이터 설정
	AnimatorController::LoadAnimatorController(L"Player/KillDong/killdong_idle_AnimController.json", animController);
	animInstance = owner->GetComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Player/KillDong/killdong_idle_sprites.json");
	animInstance->LoadAnimationClip(L"Player/KillDong/killdong_idle_anim.json");
	animInstance->ApplyClipDurationsToStates();
	animInstance->Play();
	animInstance->OnStart();

	// 콜라이더 설정
	owner->GetComponent<Collider>()->SetBoxSize(FVector2(35, 10));
	if (auto collider = owner->GetComponent<Collider>())
	{
		collider->SetLayer(0);
		collider->boxComponent->SetRelativePosition(FVector2(0, -20));
	}

	// 인풋 컴포넌트
	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });
}

void PlayerManager::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	float playerDeltaSeconds = deltaSeconds * playerTimeScale;

	if (Input::IsKeyDown(VK_S))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if(st && st->zPos > minZ)
			st->zPos -= 100.0f * playerDeltaSeconds;
	}
	if (Input::IsKeyDown(VK_W))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if (st && st->zPos < maxZ)
			st->zPos += 100.0f * playerDeltaSeconds;
	}

	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
}

void PlayerManager::SetZClamp(float _min, float _max)
{
	_min = minZ;
	_max = maxZ;
}
 
void PlayerManager::Input()
{
	// 여기에 Input에 대한 로직 작성

	if (Input::IsKeyDown(VK_8))
	{
		owner->RemoveComponent<Collider>(owner->GetComponent<Collider>());
	}
	if (Input::IsKeyDown(VK_9))
	{
		if (owner->GetComponent<Collider>() == nullptr)
			owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 60));
	}


	// 산데비스탄 테스트
	if (Input::IsKeyPressed(VK_G)) {
		if (auto prism = owner->GetComponent<Prism>())
		{
			prism->SetActive(!prism->IsActive());
			TimerManager::GetInstance().SetGlobalTimeScale(0.5);
			playerTimeScale = 2.0f;
		}
	}
}
