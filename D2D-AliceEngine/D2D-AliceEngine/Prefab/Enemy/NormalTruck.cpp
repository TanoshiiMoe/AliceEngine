#include "NormalTruck.h"
#include <System/ScriptSystem.h>
#include "Object/gameObject.h"
#include "Animation/AnimationController.h"
#include "Animation/AnimatorInstance.h"
#include "Component/SpriteRenderer.h"
#include "Component/SkewTransform.h"

void NormalTruck::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void NormalTruck::OnStart()
{
	__super::OnStart();

	FVector2 scale = { 0.6f, 0.6f };

	if (auto anim = owner->GetComponent<AnimatorInstance>()) {
		AnimatorController::LoadAnimatorController(L"Enemy/Porter/porter_sprite_AnimController.json", animController);
		anim->SetAnimatorController(&animController);
		anim->LoadSpriteSheet(L"Enemy/Porter/poter_sprites.json");
		anim->LoadAnimationClip(L"Enemy/Porter/poter_idle_anim.json");
		anim->ApplyClipDurationsToStates();

		anim->Play();
		anim->OnStart();

		// 스케일 조정
		anim->SetRelativeScale(scale);

		float height = anim->GetSpriteSizeY() * scale.y;
		float yOffset = height * 0.3f;

		SkewTransform* st = owner->GetComponent<SkewTransform>();
		st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));
		//owner->transform()->SetWorldPosition(pos);
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Porter/porter.png");
	}
}
