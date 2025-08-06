#include "PlayerBike.h"
#include "Object/gameObject.h"
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
	owner->AddComponent<Player>();
	owner->AddComponent<BackGroundRender>();
	owner->AddComponent<SkewTransform>();
	owner->AddComponent<BikeMovementScript>();
	owner->AddComponent<InputComponent>();

	// 게임 스크립트 컴포넌트
	owner->AddComponent<PlayerManager>();
}

