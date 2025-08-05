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

	// ���⿡ ������Ʈ �߰�
	AddComponent<Player>();
	AddComponent<BackGroundRender>();
	AddComponent<SkewTransform>();
	AddComponent<BikeMovementScript>();
	AddComponent<InputComponent>();
	AddComponent<PlayerManager>();
}

