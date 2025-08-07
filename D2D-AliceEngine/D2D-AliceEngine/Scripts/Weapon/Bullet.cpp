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

void Bullet::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
	owner->AddComponent<SpriteRenderer>()->LoadData(L"wallet.png");
}

void Bullet::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void Bullet::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// ���⿡ Update�� ���� ���� �ۼ�
	time += deltaSeconds;
	if (time >= duration)
	{
		isActive = false;
		return;
	}

	float t = time / duration;
	FVector2 pos = Math::QuadraticBezier<float>(P0, P1, P2, t);

	// �׸��� ��ġ ���
	GetOwner()->transform()->SetPosition(pos);
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
