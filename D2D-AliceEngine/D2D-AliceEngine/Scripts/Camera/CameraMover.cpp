#include "CameraMover.h"
#include "Manager/UpdateTaskManager.h"
#include <d2dengine_pch.cpp>
#include "../Bike/BikeMovementScript.h"
#include "Manager/SceneManager.h"
#include <Scripts/CameraController.h>
#include "Component/SkewTransform.h"

void CameraMover::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
}

void CameraMover::Awake()
{
	ownerST = owner->AddComponent<SkewTransform>();
}

void CameraMover::OnStart()
{
	//owner->AddComponent<CameraController>();
	// �÷��̾� ã�Ƽ� �ֱ�
	WeakObjectPtr<gameObject> player = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");

	FVector2 initPos;

	camera = SceneManager::GetInstance().GetCamera();

	if (player) {
		initPos = player->transform()->GetPosition();
		playerST = player->GetComponent<SkewTransform>();
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
	FVector2 cameraPos = camera->GetPosition();  // camera�� �� ��ũ��Ʈ�� owner��

	FVector2 delta = targetPos - cameraPos;

	// deadZone �ȿ� ������ �������� ����
	if (fabs(delta.x) < deadZoneX) delta.x = 0.0f;
	if (fabs(delta.y) < deadZoneY) delta.y = 0.0f;

	// ������ ������� (���� ���� ���)
	FVector2 newPos = cameraPos + delta * dt * lerpSpeed;
	camera->SetRelativePosition(targetPos);
}