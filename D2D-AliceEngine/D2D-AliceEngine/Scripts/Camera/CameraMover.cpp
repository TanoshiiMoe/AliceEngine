#include "CameraMover.h"
#include "Manager/UpdateTaskManager.h"
#include <d2dengine_pch.cpp>
#include "../Bike/BikeMovementScript.h"
#include "Manager/SceneManager.h"
#include <Scripts/CameraController.h>

void CameraMover::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
}

void CameraMover::OnStart()
{
	//owner->AddComponent<CameraController>();
	// �÷��̾� ã�Ƽ� �ֱ�
	player = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");

	FVector2 initPos;

	camera = SceneManager::GetInstance().GetCamera();

	if (player) {
		initPos = player->transform()->GetPosition();
		playerBM = player->GetComponent<BikeMovementScript>();
	}


	xPos = initPos.x;
	yPos = initPos.y;

	owner->transform()->SetPosition(xPos, 0.0f);
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


	if (!player) return;

	FVector2 targetPos = player->transform()->GetPosition();
	FVector2 cameraPos = camera->GetPosition();  // camera�� �� ��ũ��Ʈ�� owner��

	FVector2 delta = targetPos - cameraPos;

	// deadZone �ȿ� ������ �������� ����
	if (fabs(delta.x) < deadZoneX) delta.x = 0.0f;
	if (fabs(delta.y) < deadZoneY) delta.y = 0.0f;

	// ������ ������� (���� ���� ���)
	FVector2 newPos = cameraPos + delta * dt * lerpSpeed;
	camera->SetRelativePosition(newPos);
}

void CameraMover::SetPlayer(gameObject* _player)
{
	player = _player;
}
