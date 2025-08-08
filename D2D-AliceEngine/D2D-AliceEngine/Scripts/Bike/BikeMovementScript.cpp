#include "BikeMovementScript.h"
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

void BikeMovementScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
}

void BikeMovementScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�
}

void BikeMovementScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�
	// ���ο�/�ν�Ʈ ȿ�� �ð� ����
	if (m_modifierDuration > 0.0f)
	{
		m_modifierDuration -= deltaSeconds;
		if (m_modifierDuration <= 0.0f)
		{
			m_speedModifier = 1.0f;
			m_modifierDuration = 0.0f;
		}
	}

	// �浹 ���� ó��
	if (m_hitReaction)
	{
		m_hitTimer -= deltaSeconds;
		if (m_hitTimer <= 0.0f)
		{
			m_hitReaction = false;
			m_hitTimer = 0.0f;
		}
		return; // ���� ���� �� ����
	}

	// ���ӵ� ����
	m_currentSpeed += m_acceleration * deltaSeconds;
	m_currentSpeed = min(m_currentSpeed, m_maxSpeed);

	// ���� �ӵ� = ����ӵ� * ȿ������
	float finalSpeed = m_currentSpeed * m_speedModifier;
	m_prevMoveAmount = finalSpeed;
	// ���������� �̵�
	if (auto transform = m_owner->transform())
		transform->AddPosition(finalSpeed * deltaSeconds, 0);
}

void BikeMovementScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void BikeMovementScript::Awake()
{
}

void BikeMovementScript::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();
	m_currentSpeed = m_initialSpeed;

}

void BikeMovementScript::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void BikeMovementScript::OnDestroy()
{
}

void BikeMovementScript::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D ȣ���\n");

	m_hitReaction = true;
	m_hitTimer = m_hitReactionTime;
}

void BikeMovementScript::OnCollisionStay2D(Collision2D* collider)
{
	/*std::cout << "OnCollisionStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D ȣ���\n");*/
}

void BikeMovementScript::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D ȣ���\n");
}

void BikeMovementScript::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D ȣ���\n");
}

void BikeMovementScript::OnTriggerStay2D(Collider* collider)
{
	/*std::cout << "OnTriggerStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D ȣ���\n");*/
}

void BikeMovementScript::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D ȣ���\n");
}

void BikeMovementScript::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}


void BikeMovementScript::ApplySlow(float slowFactor, float duration)
{
	m_speedModifier = slowFactor;      // ��: 0.5f �� ���� �ӵ�
	m_modifierDuration = duration;
}

void BikeMovementScript::ApplyBoost(float boostFactor, float duration)
{
	m_speedModifier = boostFactor;     // ��: 1.5f �� 50% ����
	m_modifierDuration = duration;
}
