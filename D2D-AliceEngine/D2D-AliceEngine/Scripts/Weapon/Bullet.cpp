#include "Bullet.h"
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
#include <Helpers/CoordHelper.h>
#include <Component/SkewTransform.h>
#include <GameManager/BulletManager.h>
#include <Component/Collider.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Bike/BikeStatScript.h>
#include <Prefab/Enemy/Core/Car.h>
#include <Scripts/Enemy/EnemyStatScript.h>
#include "../Player/PlayerManager.h"

void Bullet::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);

	if (Collider* co = owner->AddComponent<Collider>())
	{
		co->SetBoxSize(FVector2(40, 40));
		//co->SetLayer(0);
	}

	TimerManager::GetInstance().ClearTimer(handle); // 이전 타이머 제거
	TimerManager::GetInstance().SetTimer(handle, [weak = WeakFromThis<Bullet>()]() { if(!weak.expired()) weak->bCameraCulling = true; }, 0, false, 1.0f);
}

void Bullet::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Bullet::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	UpdatePositionByType(deltaSeconds);
	UpdateCameraCulling(); // 카메라 컬링 체크
}

void Bullet::UpdatePositionByType(const float& deltaSeconds)
{
	FVector2 acceleration;
	if (WeakObjectPtr<gameObject> player = BulletManager::GetInstance().GetPlayer())
	{
		if (BikeMovementScript* bms = player->GetComponent<BikeMovementScript>())
		{
			acceleration += bms->GetPrevMoveAmount();
		}
	}
	switch (bulletType)
	{
	case EBulletType::Linear:
	{
		// 직선 이동
		FVector2 pos = moveDir * inheritedVelocity.x * moveSpeed * deltaSeconds;
		GetOwner()->transform()->AddPosition(pos);
		break;
	}
	case EBulletType::BezierCurve:
	{
		if (!bBezierFinished)
		{
			time += deltaSeconds;

			float t = time / duration;
			if (t >= 1.0f)
			{
				t = 1.0f;
				bBezierFinished = true;
			}

			FVector2 bezierPos = Math::QuadraticBezier<float>(P0, P1, P2, t);
			GetOwner()->transform()->SetPosition(bezierPos);
		}
		else
		{
			// 직선 이동
			FVector2 pos = inheritedVelocity * moveDir * deltaSeconds;
			GetOwner()->transform()->AddPosition(pos);
		}
		break;
	}
	case EBulletType::SinCurve:
	{
		time += deltaSeconds;

		// 앞으로 나아가기
		FVector2 forward = moveDir * (moveSpeed * deltaSeconds);
		currentPos += forward;

		// 수직 방향 흔들림 적용
		FVector2 perp(moveDir.y, -moveDir.x); // 정확한 수직 벡터
		float wave = std::sin(time * waveFrequency) * waveAmplitude;
		FVector2 offset = perp * wave;

		FVector2 finalPos = currentPos + offset;
		GetOwner()->transform()->SetPosition(finalPos);
		break;
	}
	default:
		break;
	}

}

void Bullet::UpdateCameraCulling()
{
	if (!bCameraCulling) return;
	FVector2 camPos = GetCamera()->GetPosition(); // Unity 좌표
	float halfW = Define::SCREEN_WIDTH * 0.5f;
	float halfH = Define::SCREEN_HEIGHT * 0.5f;
	FVector2 bulletPos = GetOwner()->transform()->GetPosition();

	float marginX = 100.0f;
	float marginY = 50.0f;
	if (bulletPos.x < camPos.x - halfW - marginX || bulletPos.x > camPos.x + halfW + marginX ||
		bulletPos.y < camPos.y - halfH - marginY || bulletPos.y > camPos.y + halfH + marginY)
	{
		isActive = false;
		GetWorld()->RemoveObject(GetOwner());
	}
}

void Bullet::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Bullet::Awake()
{
}

void Bullet::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

	SpriteRenderer* sp = owner->AddComponent<SpriteRenderer>();
	sp->LoadData(spritePath);
	sp->m_layer = 20002;

	const float angleRad = std::atan2f(moveDir.y, moveDir.x);
	const float angleDeg = angleRad * 180.0f / Define::PI;
	sp->SetRelativeRotation(angleDeg);
}

void Bullet::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void Bullet::OnDestroy()
{
	TimerManager::GetInstance().ClearTimer(handle);
}

void Bullet::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");

	if (!collider->GetOwner()) return;
	switch (droneType)
	{
	case EDroneType::Player:
		if (collider->GetOwner()->GetTag() == L"Enemy")
		{
			if (EnemyStatScript* bs = collider->GetOwner()->GetComponent<EnemyStatScript>())
			{
				// Bullet의 damage 변수 사용
				bs->m_enemyStat->DecreaseAbility("HP", damage);
			}
			GetWorld()->RemoveObject(GetOwner());
		}
		break;
	case EDroneType::Enemy:
		if (collider->GetOwner()->GetTag() == L"Player")
		{
			if (BikeStatScript* bs = collider->GetOwner()->GetComponent<BikeStatScript>())
			{
				// Bullet의 damage 변수 사용
				bs->m_bikeStat->DecreaseAbility("HP", damage);
			}
			GetWorld()->RemoveObject(GetOwner());
		}
		break;
	case EDroneType::BossSpawn:
		if (collider->GetOwner()->GetTag() == L"Player")
		{
			if (BikeStatScript* bs = collider->GetOwner()->GetComponent<BikeStatScript>())
			{
				// Bullet의 damage 변수 사용
				if(!PlayerManager::instance->GetInvincible())
					bs->m_bikeStat->DecreaseAbility("HP", damage);
			}
			GetWorld()->RemoveObject(GetOwner());
		}
		break;
	case EDroneType::Boss:
		if (collider->GetOwner()->GetTag() == L"Player")
		{
			if (BikeStatScript* bs = collider->GetOwner()->GetComponent<BikeStatScript>())
			{
				// Bullet의 damage 변수 사용
				if (!PlayerManager::instance->GetInvincible())
					bs->m_bikeStat->DecreaseAbility("HP", damage);
			}
			GetWorld()->RemoveObject(GetOwner());
		}
		break;
	default:
		break;
	}
	
}

void Bullet::OnTriggerStay2D(Collider* collider)
{
	//std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	//OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void Bullet::OnTriggerExit2D(Collider* collider)
{
	//std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	//OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void Bullet::Input()
{
	// 여기에 Input에 대한 로직 작성
}
