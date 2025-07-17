#include "ScriptExample.h"
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

void ScriptExample::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void ScriptExample::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void ScriptExample::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

}

void ScriptExample::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void ScriptExample::Awake()
{
}

void ScriptExample::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();

}

void ScriptExample::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void ScriptExample::OnDestroy()
{
}

void ScriptExample::OnCollisionEnter2D(Collision2D* collider)
{
    std::cout << "OnCollisionEnter2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnCollisionEnter2D ȣ���\n");
}

void ScriptExample::OnCollisionStay2D(Collision2D* collider)
{
    std::cout << "OnCollisionStay2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnCollisionStay2D ȣ���\n");
}

void ScriptExample::OnCollisionExit2D(Collision2D* collider)
{
    std::cout << "OnCollisionExit2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnCollisionExit2D ȣ���\n");
}

void ScriptExample::OnTriggerEnter2D(Collider* collider)
{
    std::cout << "OnTriggerEnter2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnTriggerEnter2D ȣ���\n");
}

void ScriptExample::OnTriggerStay2D(Collider* collider)
{
    std::cout << "OnTriggerStay2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnTriggerStay2D ȣ���\n");
}

void ScriptExample::OnTriggerExit2D(Collider* collider)
{
    std::cout << "OnTriggerExit2D ȣ���" << std::endl;
    OutputDebugStringW(L"OnTriggerExit2D ȣ���\n");
}

void ScriptExample::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
}
