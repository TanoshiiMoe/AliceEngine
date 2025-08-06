#include "EnemyBike.h"
#include <Scripts/Bike/LaneController.h>

void EnemyBike::Initialize()
{
	__super::Initialize();

	// 여기에 컴포넌트 추가
	AddComponent<LaneController>();
}
