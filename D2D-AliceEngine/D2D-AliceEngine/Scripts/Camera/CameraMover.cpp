#include "CameraMover.h"
#include "Manager/UpdateTaskManager.h"
#include <d2dengine_pch.cpp>
#include "../Bike/BikeMovementScript.h"
#include "Manager/SceneManager.h"

void CameraMover::Initialize()
{
	REGISTER_SCRIPT_METHOD(OnStart);

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
		{
			if (auto sp = weak.lock())
			{
				sp->Update(dt);
			}
		}
	);
}

void CameraMover::OnStart()
{
	// 플레이어 찾아서 넣기
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
	xPos += playerBM->GetCurrSpeed() * playerBM->GetSpeedModifier() * dt;
	//xPos = player->transform()->GetPosition().x;

	camera->m_transform->SetPosition(xPos, yPos);
}

void CameraMover::SetPlayer(gameObject* _player)
{
	player = _player;
}
