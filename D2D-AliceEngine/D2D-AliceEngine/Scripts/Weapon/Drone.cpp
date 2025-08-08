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
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Drone::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// ���⿡ Update�� ���� ���� �ۼ�
	// Skew���� �������.
	FVector2 bodyPos = arm->GetRelativePosition();
	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // ���콺�� ���� ���� ��ǥ
	FVector2 dir = worldMousePos - bodyPos;
	FVector2 dirNormal = dir.Normalize();

	// ����(����) ���
	float angleRad = atan2(dirNormal.y, dirNormal.x);
	// ������ ��(degree)�� ��ȯ (������ degree �������)
	float angleDeg = angleRad * (180.0f / PI);
	arm->SetRelativeRotation(angleDeg);

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
	body->SetRelativePosition(initBodyPos + FVector2(0,pos.y));


	if (Input::IsMouseLeftDown() && bCanFire)
	{
		FVector2 cameraPos = GetCamera()->GetPosition();
		float currentSpeed = owner->GetComponent<BikeMovementScript>()->GetCurrSpeed();
		FVector2 speed{ currentSpeed , 0 };
		BulletManager::GetInstance().FireBullet(bodyPos, worldMousePos, speed);
		bCanFire = false;
		TimerManager::GetInstance().SetGlobalTimeScale(1);
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
			OutputDebugStringW(L"���� Ÿ�̸� ȣ���!\n");
		},
		0.1f,
		true,
		0.0f
	);

	body = owner->AddComponent<SpriteRenderer>();
	body->LoadData(L"Enemy/drone/drone_killdong_body.png");
	body->SetRelativeScale(FVector2(0.7f, 0.7f));
	body->SetRelativePosition(initBodyPos);

	arm = owner->AddComponent<SpriteRenderer>();
	arm->LoadData(L"Enemy/drone/drone_killdong_arm.png");
	arm->SetRelativeScale(FVector2(1.0f, 1.0f));
	arm->RemoveFromParent();
	body->AddChildComponent(arm);
}

void Drone::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Drone::OnDestroy()
{
}

void Drone::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D ȣ���\n");
}

void Drone::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "OnCollisionStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D ȣ���\n");
}

void Drone::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D ȣ���\n");
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
