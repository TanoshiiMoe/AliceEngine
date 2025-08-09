#include "Drone.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>
#include <Scripts/Weapon/BulletManager.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Component/SkewTransform.h>
#include <GameManager/GamePlayManager.h>

Drone::Drone(FDroneSpritePath path)
{
	spritePath = path;
}

void Drone::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);

	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bWaitForSecond = false;
		},
		0.0f,
		false,
		0.2f
	);
}

void Drone::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Drone::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!GamePlayManager::GetInstance().IsPlaying()) return;
	if (bWaitForSecond) return;

	// 여기에 Update에 대한 로직 작성
	// Skew랑은 상관없음.
	if (!arm) return;
	FVector2 bodyPos = arm->GetRelativePosition();
	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
	FVector2 dir = worldMousePos - bodyPos;
	FVector2 dirNormal = dir.Normalize();
	
	Floating(deltaSeconds, dirNormal);
	AttackAction(bodyPos, worldMousePos, dirNormal);
}

void Drone::Floating(const float& deltaSeconds, const FVector2& dirNormal)
{
	//body->SetRelativePosition(FVector2(27, 6));
	//// 시간 누적
	elapsed += deltaSeconds;
	// 구간이 끝나면 방향 전환
	if (elapsed >= duration)
	{
		elapsed = 0.0f;
		goingUp = !goingUp;
	}

	float currY;
	if (goingUp)
	{
		// 위로 이동
		currY = Math::EaseInOut(startY, endY, elapsed, duration);
	}
	else
	{
		// 아래로 이동
		currY = Math::EaseInOut(endY, startY, elapsed, duration);
	}

	// 오브젝트 Y좌표 적용
	auto pos = body->GetRelativePosition();
	pos.y = currY;
	body->SetRelativePosition(initBodyPos + FVector2(0, pos.y));
}

void Drone::AttackAction(const FVector2& bodyPos, const FVector2& worldMousePos, const FVector2& dirNormal)
{
	switch (droneType)
	{
	case EDroneType::Player:
	{
		// float 버전 사용 + 항상 초기화
		const float angleRad = std::atan2f(dirNormal.y, dirNormal.x);
		const float angleDeg = angleRad * 180.0f / Define::PI + armDegree;
		arm->SetRelativeRotation(angleDeg);

		if (Input::IsMouseLeftDown() && bCanFire)
		{
			float currentSpeed = 0.0f;
			if (auto bike = owner->GetComponent<BikeMovementScript>())
				currentSpeed = bike->GetCurrSpeed();

			const FVector2 speed{ currentSpeed, 0.0f };
			BulletManager::GetInstance().FireBullet(bodyPos, worldMousePos, speed, droneType);

			bCanFire = false;
		}
		break;
	}
	case EDroneType::Enemy:
	{
		if (auto player = BulletManager::GetInstance().GetPlayer())
		{
			// 1발짜리
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2(0,0);
			////	: FVector2{ 0.0f, 0.0f };
			//float currentSpeed = 0.0f;
			//FVector2 dir = bodyPos - targetPos;
			//FVector2 _dirNormal = dir.Normalize();
			//// float 버전 사용 + 항상 초기화
			//const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			//const float angleDeg = angleRad * 180.0f / Define::PI;
			//arm->SetRelativeRotation(angleDeg);

			//if (bCanFire)
			//{
			//	if (auto bike = player->GetComponent<BikeMovementScript>())
			//	{
			//		// 안전하게 접근
			//		if (auto ownerGO = bike->GetOwner())
			//			targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

			//		currentSpeed = bike->GetCurrSpeed();
			//	}

			//	const FVector2 speed{ currentSpeed, 0.0f };
			//	BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);
			//	bCanFire = false;
			//}

			//// 3발짜리
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
			//	: FVector2{ 0.0f, 0.0f };
			//float currentSpeed = 0.0f;

			//FVector2 dir = bodyPos - targetPos; // 적 → 플레이어 방향
			//FVector2 dirNormal = dir.Normalize();

			//const float angleRad = std::atan2f(dirNormal.y, dirNormal.x);
			//const float angleDeg = angleRad * 180.0f / Define::PI + armDegree;;
			//arm->SetRelativeRotation(angleDeg);

			//if (bCanFire)
			//{
			//	if (auto bike = player->GetComponent<BikeMovementScript>())
			//	{
			//		if (auto ownerGO = bike->GetOwner())
			//			targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

			//		currentSpeed = bike->GetCurrSpeed();
			//	}

			//	const FVector2 speed{ currentSpeed, 0.0f };

			//	// 0도(원본)
			//	BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);

			//	// 회전 함수
			//	auto RotateVector = [](const FVector2& v, float degree) -> FVector2 {
			//		float rad = degree * Define::PI / 180.0f;
			//		float cs = std::cos(rad);
			//		float sn = std::sin(rad);
			//		return {   v.y * sn - v.x * cs, v.y * cs - v.x * sn };
			//		};

			//	// +30도
			//	{
			//		FVector2 dir30 = RotateVector(dirNormal, 30.0f);
			//		FVector2 newTarget = bodyPos + dir30 * 1000.0f; // 먼 위치로 타겟
			//		BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
			//	}

			//	// -30도
			//	{
			//		FVector2 dirNeg30 = RotateVector(dirNormal, -30.0f);
			//		FVector2 newTarget = bodyPos + dirNeg30 * 1000.0f;
			//		BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
			//	}

			//	bCanFire = false;
			//}
			// 
			// 평시 포신은 플레이어 대략 조준(시각용)

			// 동그랗게 3발 
			FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2{ 0,0 };
			FVector2 dir = targetPos - bodyPos ;
			FVector2 _dirNormal = -dir.Normalize();
			// arm 회전
			const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			const float angleDeg = angleRad * 180.0f / Define::PI;
			if (arm) arm->SetRelativeRotation(angleDeg);

			 //bCanFire가 true일 때만 버스트 시작(1회에 3발)
            if (bCanFire)
            {
                bCanFire = false;       // 외부 쿨다운 타이머로 다시 켜짐(기존 OnStart의 timer)
                burstRemaining = 3;     // 연속 3발
                // 첫 발 즉시 발사
                FireOneBurstShot();
                // 이후 남은 발은 간격을 두고 순차 발사
                if (burstRemaining > 0)
                {
                    TimerManager::GetInstance().SetTimer(
                        burstTimer,
                        [this]() { FireOneBurstShot(); },
                        burstInterval,
                        true,              // 루프: 남은 발이 0이 되면 내부에서 타이머 정리
                        burstInterval      // 첫 지연도 동일 간격으로 설정
                    );
                }
            }
		}
		break;
	}
	default:
		break;
	}
}

// 한 발 쏘고, 남았으면 다음 발 타이머 예약
void Drone::FireOneBurstShot()
{
	auto player = BulletManager::GetInstance().GetPlayer();
	if (!player) { burstRemaining = 0; return; }

	// 현재 위치들 갱신
	const FVector2 bodyPos = arm ? arm->GetRelativePosition() : FVector2{ 0,0 };
	FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2{ 0,0 };

	// 랜덤 목표점(플레이어 주변 원)
	const auto rnd = FRandom::GetRandomPointInCircle2D(targetPos.x, targetPos.y, spreadRadius);
	const FVector2 aimedTarget{ rnd.x, rnd.y };

	// 속도 반영(플레이어 바이크 이동속도)
	float currentSpeed = 0.0f;
	if (auto bike = player->GetComponent<BikeMovementScript>())
		currentSpeed = bike->GetCurrSpeed();

	const FVector2 speed{ currentSpeed, 0.0f };

	// 발사
	BulletManager::GetInstance().FireBullet(bodyPos, aimedTarget, speed, droneType);

	// 남은 발 관리 및 다음 타이머
    if (--burstRemaining <= 0)
    {
        // 더 이상 남은 발이 없으면 반복 타이머 중지
        TimerManager::GetInstance().ClearTimer(burstTimer);
    }
}

void Drone::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Drone::Awake()
{
}

void Drone::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bCanFire = true;
		},
		attackDelay,
		true,
		0.0f
	);

	body = owner->AddComponent<SpriteRenderer>();
	body->m_layer = 10000;
	body->LoadData(spritePath.body);
	body->SetRelativeScale(initBodySize);
	body->SetRelativePosition(initBodyPos);

	arm = owner->AddComponent<SpriteRenderer>();
	arm->m_layer = 10001;
	arm->LoadData(spritePath.arm);
	arm->SetRelativeScale(FVector2(1.0f, 1.0f));
	arm->RemoveFromParent();
	body->AddChildComponent(arm);
}

void Drone::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().ClearTimer(burstTimer);
}

void Drone::OnDestroy()
{
	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().ClearTimer(burstTimer);
}

void Drone::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void Drone::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void Drone::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void Drone::Input()
{
	// 여기에 Input에 대한 로직 작성
}
