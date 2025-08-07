#include "Bullet.h"
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
#include <scripts/Weapon/BulletManager.h>
#include <Component/Collider.h>
#include <Scripts/Bike/BikeMovementScript.h>

void Bullet::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
	SpriteRenderer* sp = owner->AddComponent<SpriteRenderer>();
	sp->LoadData(L"wallet.png");
	sp->m_layer = 20000;

	
	if (Collider* co = owner->AddComponent<Collider>())
	{
		co->SetBoxSize(FVector2(40, 40));
		co->SetLayer(-10);
	}

	TimerManager::GetInstance().ClearTimer(handle); // ���� Ÿ�̸� ����
	TimerManager::GetInstance().SetTimer(handle, [this]() { bCameraCulling = true; }, 0, false, 1.0f);
}

void Bullet::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Bullet::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	UpdatePositionByType(deltaSeconds);
	UpdateCameraCulling(); // ī�޶� �ø� üũ
}

void Bullet::UpdatePositionByType(const float& deltaSeconds)
{
	//FVector2 acceleration;
	//if (BikeMovementScript* bms = owner->GetComponent<BikeMovementScript>())
	//{
	//	acceleration += bms->GetAcceleration();
	//}
	switch (bulletType)
	{
	case EBulletType::Linear:
	{
		// ���� �̵�
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
			// ���� �̵�
			FVector2 pos = inheritedVelocity * moveDir * deltaSeconds;
			GetOwner()->transform()->AddPosition(pos);
		}
		break;
	}
	case EBulletType::SinCurve:
	{
		time += deltaSeconds;

		// ������ ���ư���
		FVector2 forward = moveDir * (moveSpeed * deltaSeconds);
		currentPos += forward;

		// ���� ���� ��鸲 ����
		FVector2 perp(moveDir.y, -moveDir.x); // ��Ȯ�� ���� ����
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
	FVector2 camPos = GetCamera()->GetPosition(); // Unity ��ǥ
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
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void Bullet::Awake()
{
}

void Bullet::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();
	
}

void Bullet::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void Bullet::OnDestroy()
{
	TimerManager::GetInstance().ClearTimer(handle);
}

void Bullet::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D ȣ���\n");
}

void Bullet::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "OnCollisionStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D ȣ���\n");
}

void Bullet::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D ȣ���\n");
}

void Bullet::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D ȣ���\n");
}

void Bullet::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D ȣ���\n");
}

void Bullet::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D ȣ���\n");
}

void Bullet::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
