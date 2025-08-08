#include "Car.h"
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"
#include "System/ScriptSystem.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimatorInstance.h"
#include <Object/Camera.h>
#include <Manager/SceneManager.h>

void Car::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	//애니메이터 있을시
	//owner->AddComponent<Animator>();
	// �ִϸ����� ������
	owner->AddComponent<AnimatorInstance>();
	// ������
	owner->AddComponent<SpriteRenderer>();

	owner->AddComponent<SkewTransform>();
	owner->AddComponent<LaneController>();

	// 스크립트
	owner->AddComponent<EnemyManager>();

	TimerManager::GetInstance().ClearTimer(timer);
	// 2초에 한번씩 카메라 컬링 체크
	TimerManager::GetInstance().SetTimer(
		timer,
		[weak = WeakFromThis<Car>()]() {
			if (!weak) return;
			FVector2 camPos = weak->GetCamera()->GetPosition(); // Unity 좌표
			float halfW = Define::SCREEN_WIDTH * 0.5f;
			float halfH = Define::SCREEN_HEIGHT * 0.5f;
			FVector2 bulletPos = weak->GetOwner()->transform()->GetPosition();

			float marginX = 100.0f;
			float marginY = 150.0f;
			if (bulletPos.x < camPos.x - halfW - marginX || bulletPos.x > camPos.x + halfW + marginX ||
				bulletPos.y < camPos.y - halfH - marginY || bulletPos.y > camPos.y + halfH + marginY)
			{
				weak->GetWorld()->RemoveObject(weak->GetOwner());
			}
		},
		2.0f,
		true,
		0.5f);
}

void Car::OnStart()
{
	// SkewTransform으로 변환하기
	SkewTransform* st = owner->GetComponent<SkewTransform>();
	st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");
	st->ToSkewPos();
}
