#include "NormalTruck.h"
#include <System/ScriptSystem.h>
#include "Object/gameObject.h"
#include "Animation/AnimationController.h"
#include "Animation/AnimatorInstance.h"
#include "Component/SpriteRenderer.h"

void NormalTruck::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void NormalTruck::OnStart()
{
	__super::OnStart();
	if (auto anim = owner->GetComponent<AnimatorInstance>()) {
		AnimatorController::LoadAnimatorController(L"Enemy/Porter/porter_sprite_AnimController.json", animController);
		anim->SetAnimatorController(&animController);
		anim->LoadSpriteSheet(L"Enemy/Porter/poter_sprites.json");
		anim->LoadAnimationClip(L"Enemy/Porter/poter_idle_anim.json");
		anim->ApplyClipDurationsToStates();

		anim->Play();
		anim->OnStart();
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Porter/porter.png");
	}

	owner->transform()->SetScale(0.6f, 0.6f);
	
}
