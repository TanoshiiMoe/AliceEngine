#include "CameraMover.h"
#include "Manager/UpdateTaskManager.h"
#include <d2dengine_pch.cpp>
#include "../Bike/BikeMovementScript.h"
#include "Manager/SceneManager.h"
#include "Component/SkewTransform.h"

void CameraMover::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
}

void CameraMover::Awake()
{
}

void CameraMover::OnStart()
{
	//owner->AddComponent<CameraController>();
	// 플레이어 찾아서 넣기
	WeakObjectPtr<gameObject> player = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");

	FVector2 initPos;

	camera = GetCamera();

	if (player) {
		initPos = player->transform()->GetPosition();
		playerST = player->GetComponent<SkewTransform>();

		player->RemoveFromParent();
		player->AddChildTransform(&GetCamera()->relativeTransform);
		GetCamera()->SetRelativeScale(player->GetScaleInv());
		GetCamera()->SetRelativePosition(FVector2(0, 0));
		GetCamera()->RemoveFromParent();
	}

	xPos = initPos.x;
	yPos = initPos.y;
}

void CameraMover::Update(const float& dt)
{
	////xPos += playerBM->GetCurrSpeed() * playerBM->GetSpeedModifier() * dt;
	//float t1 = playerBM->GetCurrSpeed();
	//float t2 = playerBM->GetSpeedModifier();
	////xPos += playerBM->GetCurrSpeed() * playerBM->GetSpeedModifier() * dt;
	//xPos = player->transform()->GetPosition().x;
	//
	////camera->SetPosition(xPos, yPos);
	//camera->SetPosition(xPos, yPos);


	if (!playerST) return;

	FVector2 targetPos = playerST->GetRealPos();
	FVector2 cameraPos = camera->GetPosition();  // camera는 이 스크립트의 owner임

	FVector2 delta = targetPos - cameraPos;

	// deadZone 안에 있으면 움직이지 않음
	if (fabs(delta.x) < deadZoneX) delta.x = 0.0f;
	if (fabs(delta.y) < deadZoneY) delta.y = 0.0f;

	// 느리게 따라오기 (선형 보간 방식)
	FVector2 newPos = cameraPos + delta * dt * lerpSpeed;
	camera->SetRelativePosition(targetPos);
}
