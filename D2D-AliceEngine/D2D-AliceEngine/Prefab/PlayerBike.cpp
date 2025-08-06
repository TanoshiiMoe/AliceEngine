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

	// ���⿡ ������Ʈ �߰�	
	owner->AddComponent<Player>();
	owner->AddComponent<BackGroundRender>();
	owner->AddComponent<SkewTransform>();
	owner->AddComponent<BikeMovementScript>();
	owner->AddComponent<InputComponent>();

	// ���� ��ũ��Ʈ ������Ʈ
	owner->AddComponent<PlayerManager>();
}

