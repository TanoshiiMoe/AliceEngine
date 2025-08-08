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
			TimerManager::GetInstance().SetGlobalTimeScale(1);
		}
		break;
	}
	case EDroneType::Enemy:
	{
		if (auto player = BulletManager::GetInstance().GetPlayer())
		{
			// 1발짜리
			
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
			//	: FVector2{ 0.0f, 0.0f };
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

			// 3발짜리
			FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
				: FVector2{ 0.0f, 0.0f };
			float currentSpeed = 0.0f;

			FVector2 dir = bodyPos - targetPos; // 적 → 플레이어 방향
			FVector2 dirNormal = dir.Normalize();

			const float angleRad = std::atan2f(dirNormal.y, dirNormal.x);
			const float angleDeg = angleRad * 180.0f / Define::PI + armDegree;;
			arm->SetRelativeRotation(angleDeg);

			if (bCanFire)
			{
				if (auto bike = player->GetComponent<BikeMovementScript>())
				{
					if (auto ownerGO = bike->GetOwner())
						targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

					currentSpeed = bike->GetCurrSpeed();
				}

				const FVector2 speed{ currentSpeed, 0.0f };

				// 0도(원본)
				BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);

				// 회전 함수
				auto RotateVector = [](const FVector2& v, float degree) -> FVector2 {
					float rad = degree * Define::PI / 180.0f;
					float cs = std::cos(rad);
					float sn = std::sin(rad);
					return {   v.y * sn - v.x * cs, v.y * cs - v.x * sn };
					};

				// +30도
				{
					FVector2 dir30 = RotateVector(dirNormal, 30.0f);
					FVector2 newTarget = bodyPos + dir30 * 1000.0f; // 먼 위치로 타겟
					BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
				}

				// -30도
				{
					FVector2 dirNeg30 = RotateVector(dirNormal, -30.0f);
					FVector2 newTarget = bodyPos + dirNeg30 * 1000.0f;
					BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
				}

				bCanFire = false;
			}
		}
		break;
	}
	default:
		break;
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
}

void Drone::OnDestroy()
{
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
