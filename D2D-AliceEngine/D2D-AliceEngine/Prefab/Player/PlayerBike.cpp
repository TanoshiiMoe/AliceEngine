#include "PlayerBike.h"
#include "Object/gameObject.h"
#include <Scripts/Player.h>
#include <Scripts/BackGroundRender.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include "Component/InputComponent.h"
#include "Manager/SceneManager.h"
#include "Scripts/Player/PlayerManager.h"
#include "System/ScriptSystem.h"
#include "Animation/AnimationController.h"
#include "Animation/AnimatorInstance.h"
#include "Component/Collider.h"
#include "Effect/Prism.h"

#include <Manager/UpdateTaskManager.h>
#include <Scripts/Weapon/Drone.h>
#include <Scripts/Bike/BikeStatScript.h>

void PlayerBike::Initialize()
{
	__super::Initialize();

	// ���⿡ ������Ʈ �߰�	
	owner->AddComponent<AnimatorInstance>();
	//owner->AddComponent<BackGroundRender>();
	owner->AddComponent<SkewTransform>();
	owner->AddComponent<BikeMovementScript>();
	owner->AddComponent<Collider>();
	owner->AddComponent<InputComponent>();
	owner->AddComponent<Prism>(10, 0.1f);
	
	FDroneSpritePath dronePath(
		L"Player/drone/drone_killdong_body.png",
		L"Player/drone/drone_killdong_arm.png"
	);
	if(Drone* drone = owner->AddComponent<Drone>(dronePath))
		drone->SetDroneType(EDroneType::Player);
	owner->AddComponent<BikeStatScript>();

	// ���� ��ũ��Ʈ ������Ʈ
	owner->AddComponent<PlayerManager>();
}