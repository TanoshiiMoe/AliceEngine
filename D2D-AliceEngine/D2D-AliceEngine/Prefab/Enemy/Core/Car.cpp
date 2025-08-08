#include "Car.h"
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"
#include "System/ScriptSystem.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimatorInstance.h"

void Car::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	// 애니메이터 있을시
	owner->AddComponent<AnimatorInstance>();
	// 없을시
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// 스크립트
	owner->AddComponent<EnemyManager>();
}

void Car::OnStart()
{
	// SkewTransform으로 변환하기
	SkewTransform* st = owner->GetComponent<SkewTransform>();
	st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");
	st->ToSkewPos();
}
