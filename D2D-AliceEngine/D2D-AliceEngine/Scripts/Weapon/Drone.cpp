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
}

void Drone::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Drone::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 여기에 Update에 대한 로직 작성
	// Skew랑은 상관없음.
	if (!arm) return;
	FVector2 bodyPos = arm->GetRelativePosition();
	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
	FVector2 dir = worldMousePos - bodyPos;
	FVector2 dirNormal = dir.Normalize();
	
	Floating(deltaSeconds, dirNormal);
	MouseButton(bodyPos, worldMousePos, dirNormal);
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

void Drone::MouseButton(const FVector2& bodyPos, const FVector2& worldMousePos, const FVector2& dirNormal)
{
	if (droneType == EDroneType::Enemy) return;
	// 각도(라디안) 계산
	float angleRad = atan2(dirNormal.y, dirNormal.x);
	// 라디안을 도(degree)로 변환 (엔진이 degree 단위라면)
	float angleDeg = angleRad * (180.0f / PI);
	arm->SetRelativeRotation(angleDeg);

	if (Input::IsMouseLeftDown() && bCanFire)
	{
		FVector2 cameraPos = GetCamera()->GetPosition();
		float currentSpeed = 0;
		if (auto Bike = owner->GetComponent<BikeMovementScript>())
		{
			currentSpeed = Bike->GetCurrSpeed();
		}
		FVector2 speed{ currentSpeed , 0 };
		BulletManager::GetInstance().FireBullet(bodyPos, worldMousePos, speed);
		bCanFire = false;
		TimerManager::GetInstance().SetGlobalTimeScale(1);
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
		0.1f,
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
}

void Drone::OnDestroy()
{
}

void Drone::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D 호출됨\n");
}

void Drone::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "OnCollisionStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D 호출됨\n");
}

void Drone::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D 호출됨\n");
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
