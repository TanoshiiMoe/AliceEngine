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
	if (Input::IsMouseLeftDown() && bCanFire)
	{
		FVector2 ownerPos = owner->GetPosition();
		//owner->GetComponent<SkewTransform>()->ToSkewPos();
		//FVector2 ownerPos = owner->GetComponent<SkewTransform>()->GetRealPos();
		FVector2 cameraPos = GetCamera()->GetPosition();
		FVector2 worldMousePos = Input::GetMouseWorldPosition(); // ���콺�� ���� ���� ��ǥ
		float currentSpeed = owner->GetComponent<BikeMovementScript>()->GetCurrSpeed();
		FVector2 speed{ currentSpeed , 0 };
		BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, speed);
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
