#include "PlayerMovement.h"
#include "System/ScriptSystem.h"
#include "Manager/UpdateTaskManager.h"
#include "Manager/SceneManager.h"
#include "../Bike/BikeMovementScript.h"
#include "PlayerManager.h"
#include <Core/Input.h>
#include "Component/InputComponent.h"
#include <GameManager/GamePlayManager.h>

void PlayerMovement::Initialize()
{
	__super::Initialize();

	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void PlayerMovement::OnStart()
{
	if (!bMovement)
		bMovement = owner->AddComponent<BikeMovementScript>();

	if (bMovement) {
		initSpeed = bMovement->GetInitialSpeed();
		bMovement->SetAcceleration(200.0f);
	}

	if (auto ic = owner->GetComponent<InputComponent>())
		ic->SetAction(owner->GetHandle(), [this]() { Input(); });
	else
		OutputDebugStringW(L"플레이어의 movement 스크립트에서 인풋컴포넌트를 가져오는데 실패함!!\n");
}

void PlayerMovement::Update(const float& deltaSeconds)
{
	dt = deltaSeconds;
	bMovement->SetMaxSpeed(bMovement->GetMaxSpeed() + moveDir.x * deltaSeconds);
}

void PlayerMovement::Input()
{
	moveDir = { 0.0f, 0.0f };

	if (Input::IsKeyDown(VK_A)) {
		// initialSpeed 줄이기
		moveDir.x -= 10.0f;
	}
	if (Input::IsKeyDown(VK_D)) {
		// initialSpeed 늘리기
		moveDir.x += 10.0f;
	}
}
