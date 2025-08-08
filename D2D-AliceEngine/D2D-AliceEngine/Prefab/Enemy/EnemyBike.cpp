#include "EnemyBike.h"
#include <Scripts/Bike/LaneController.h>
#include "System/ScriptSystem.h"
#include <Component/SpriteRenderer.h>
#include "Scripts/Bike/BikeStatScript.h"
#include "Animation/AnimatorInstance.h"

void EnemyBike::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemyBike::OnStart()
{
	__super::OnStart();
	if (auto anim = owner->GetComponent<AnimatorInstance>()) {
		AnimatorController::LoadAnimatorController(L"Enemy/Bike/bike_sprite_AnimController.json", animController);
		anim->SetAnimatorController(&animController);
		anim->LoadSpriteSheet(L"Enemy/Bike/bike_sprites.json");
		anim->LoadAnimationClip(L"Enemy/Bike/bike_idle_anim.json");
		anim->ApplyClipDurationsToStates();

		anim->Play();
		anim->OnStart();
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Bike/bike.png");
	}

	owner->transform()->SetScale(0.6f, 0.6f);

	/*__super::OnStart();
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Bike/bike.png");
	owner->AddComponent<BikeStatScript>();
	owner->transform()->SetScale(0.6f, 0.6f);*/
}

