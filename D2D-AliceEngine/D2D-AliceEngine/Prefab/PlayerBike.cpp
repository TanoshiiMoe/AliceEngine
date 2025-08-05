#include "PlayerBike.h"
#include <Scripts/Player.h>
#include <Scripts/BackGroundRender.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include "Component/InputComponent.h"
#include "Manager/SceneManager.h"
#include "Scripts/Player/PlayerManager.h"

void PlayerBike::Initialize()
{
	__super::Initialize();

	// 여기에 컴포넌트 추가
	AddComponent<Player>();
	AddComponent<BackGroundRender>();
	AddComponent<SkewTransform>();
	AddComponent<BikeMovementScript>();
	AddComponent<InputComponent>();
	AddComponent<PlayerManager>();
}

