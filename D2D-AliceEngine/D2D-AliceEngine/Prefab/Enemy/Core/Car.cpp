#include "Car.h"
#include <Component/Animator.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"

void Car::Initialize()
{
	__super::Initialize();

	//애니메이터 있을시
	//owner->AddComponent<Animator>();
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// 스크립트
	owner->AddComponent<EnemyManager>();
}
