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
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Drone::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (bWaitForSecond) return;

	// ���⿡ Update�� ���� ���� �ۼ�
	// Skew���� �������.
	if (!arm) return;
	FVector2 bodyPos = arm->GetRelativePosition();
	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // ���콺�� ���� ���� ��ǥ
	FVector2 dir = worldMousePos - bodyPos;
	FVector2 dirNormal = dir.Normalize();
	
	Floating(deltaSeconds, dirNormal);
	AttackAction(bodyPos, worldMousePos, dirNormal);
}

void Drone::Floating(const float& deltaSeconds, const FVector2& dirNormal)
{
	//body->SetRelativePosition(FVector2(27, 6));
	//// �ð� ����
	elapsed += deltaSeconds;
	// ������ ������ ���� ��ȯ
	if (elapsed >= duration)
	{
		elapsed = 0.0f;
		goingUp = !goingUp;
	}

	float currY;
	if (goingUp)
	{
		// ���� �̵�
		currY = Math::EaseInOut(startY, endY, elapsed, duration);
	}
	else
	{
		// �Ʒ��� �̵�
		currY = Math::EaseInOut(endY, startY, elapsed, duration);
	}

	// ������Ʈ Y��ǥ ����
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
		// float ���� ��� + �׻� �ʱ�ȭ
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
			// 1��¥��
			
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
			//	: FVector2{ 0.0f, 0.0f };
			//float currentSpeed = 0.0f;
			//FVector2 dir = bodyPos - targetPos;
			//FVector2 _dirNormal = dir.Normalize();
			//// float ���� ��� + �׻� �ʱ�ȭ
			//const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			//const float angleDeg = angleRad * 180.0f / Define::PI;
			//arm->SetRelativeRotation(angleDeg);

			//if (bCanFire)
			//{
			//	if (auto bike = player->GetComponent<BikeMovementScript>())
			//	{
			//		// �����ϰ� ����
			//		if (auto ownerGO = bike->GetOwner())
			//			targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

			//		currentSpeed = bike->GetCurrSpeed();
			//	}

			//	const FVector2 speed{ currentSpeed, 0.0f };
			//	BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);
			//	bCanFire = false;
			//}

			// 3��¥��
			FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
				: FVector2{ 0.0f, 0.0f };
			float currentSpeed = 0.0f;

			FVector2 dir = bodyPos - targetPos; // �� �� �÷��̾� ����
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

				// 0��(����)
				BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);

				// ȸ�� �Լ�
				auto RotateVector = [](const FVector2& v, float degree) -> FVector2 {
					float rad = degree * Define::PI / 180.0f;
					float cs = std::cos(rad);
					float sn = std::sin(rad);
					return {   v.y * sn - v.x * cs, v.y * cs - v.x * sn };
					};

				// +30��
				{
					FVector2 dir30 = RotateVector(dirNormal, 30.0f);
					FVector2 newTarget = bodyPos + dir30 * 1000.0f; // �� ��ġ�� Ÿ��
					BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
				}

				// -30��
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
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Drone::Awake()
{
}

void Drone::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
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
	// ���⿡ OnEnd�� ���� ���� �ۼ�
	TimerManager::GetInstance().ClearTimer(timer);
}

void Drone::OnDestroy()
{
}

void Drone::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D ȣ���\n");
}

void Drone::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D ȣ���\n");
}

void Drone::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D ȣ���\n");
}

void Drone::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
