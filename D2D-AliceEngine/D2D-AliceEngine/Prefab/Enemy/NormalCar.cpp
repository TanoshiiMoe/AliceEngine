#include "NormalCar.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"
#include "Animation/AnimatorInstance.h"

void NormalCar::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void NormalCar::OnStart()
{
	__super::OnStart();
	if (auto anim = owner->GetComponent<AnimatorInstance>()) {
		AnimatorController::LoadAnimatorController(L"Enemy/Tico/tico_sprite_AnimController.json", animController);
		anim->SetAnimatorController(&animController);
		anim->LoadSpriteSheet(L"Enemy/Tico/tico_sprites.json");
		anim->LoadAnimationClip(L"Enemy/Tico/tico_idle_anim.json");
		anim->ApplyClipDurationsToStates();

		anim->Play();
		anim->OnStart();
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Tico/tico.png");
	}

	owner->transform()->SetScale(0.7f, 0.7f);
}
