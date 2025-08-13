#include "EnemyBike.h"
#include <Scripts/Bike/LaneController.h>
#include "System/ScriptSystem.h"
#include <Component/SpriteRenderer.h>
#include "Scripts/Bike/BikeStatScript.h"
#include "Animation/AnimatorInstance.h"
#include <Helpers/CoordHelper.h>
#include "Component/SkewTransform.h"
#include "Component/Collider.h"
#include <Component/BoxComponent.h>
#include "Scene/Scene.h"
#include "Scripts/Weapon/BulletColl.h"

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
		//anim->m_layer = 19999;

		float height = anim->GetSpriteSizeY() * scale.y;
		float yOffset = height * 0.4f;

		SkewTransform* st = owner->GetComponent<SkewTransform>();
		st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));
		//owner->transform()->SetWorldPosition(pos);
	}
	else {
		owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Bike/bike.png");
	}

	// 콜라이더 설정
	Collider* coll = owner->GetComponent<Collider>();
	coll->boxComponent->SetSize(FVector2(150, 10));
	coll->boxComponent->SetRelativePosition(FVector2(0.0f, -50.0f));

	// 총알받이 설정
	gameObject* bColObj = GetWorld()->NewObject<gameObject>(L"EnemyBikeColl");
	bColObj->SetTag(L"Enemy");
	BulletColl* bc = bColObj->AddComponent<BulletColl>();
	bc->SetTarget(owner);
	bc->SetCollSize(FVector2(150.0f, 70.0f));
	colObjs.push_back(bColObj);
}
