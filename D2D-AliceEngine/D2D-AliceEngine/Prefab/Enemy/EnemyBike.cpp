#include "EnemyBike.h"
#include <Scripts/Bike/LaneController.h>
#include "System/ScriptSystem.h"
#include <Component/SpriteRenderer.h>

void EnemyBike::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemyBike::OnStart()
{
	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Bike/bike.png");
}

