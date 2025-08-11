#include "EnemyBike.h"
#include <Scripts/Bike/LaneController.h>
#include "System/ScriptSystem.h"
#include <Component/SpriteRenderer.h>
#include "Scripts/Bike/BikeStatScript.h"
#include "Animation/AnimatorInstance.h"
#include <Helpers/CoordHelper.h>
#include "Component/SkewTransform.h"

void EnemyBike::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemyBike::OnStart()
{
	__super::OnStart();

	FVector2 scale = { 0.6f, 0.6f };

	if (auto anim = owner->GetComponent<AnimatorInstance>()) {
		AnimatorController::LoadAnimatorController(L"Enemy/Bike/bike_sprite_AnimController.json", animController);
		anim->SetAnimatorController(&animController);
		anim->LoadSpriteSheet(L"Enemy/Bike/bike_sprites.json");
		anim->LoadAnimationClip(L"Enemy/Bike/bike_idle_anim.json");
		anim->ApplyClipDurationsToStates();
		anim->Play();
		anim->OnStart();

		// ������ ����
		anim->SetRelativeScale(scale);

		float height = anim->GetSpriteSizeY() * scale.y;
		float yOffset = height * 0.4f;

		SkewTransform* st = owner->GetComponent<SkewTransform>();
		st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));
		//owner->transform()->SetWorldPosition(pos);
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Bike/bike.png");
	}
}
