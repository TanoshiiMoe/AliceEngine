#include "SpineScript.h"
#include "SpineScript.h"
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
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <System/RenderSystem.h>

void SpineScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);

	REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
}

void SpineScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void SpineScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	spineRenderer->UpdateAnimation(deltaSeconds);
}

void SpineScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void SpineScript::Awake()
{
}

void SpineScript::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();
	//RenderSystem::GetInstance().m_spineRenders.push_back({ m_owner->GetHandle(), [this]() { spineRenderer->Render(); } });
	RenderSystem::GetInstance().RegistSpine2D(m_owner->GetHandle(), [this]() { spineRenderer->Render(); });

	spineRenderer = std::make_unique<SpineRenderer>();
	spineRenderer->Initialize();
	spineRenderer->LoadSpine(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");

	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), 
		[this]() 
		{
			Input();
		});
}

void SpineScript::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void SpineScript::OnDestroy()
{
	spineRenderer->Shutdown();
	spineRenderer.reset();
}

void SpineScript::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D ȣ���\n");
}

void SpineScript::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "OnCollisionStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D ȣ���\n");
}

void SpineScript::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D ȣ���\n");
}

void SpineScript::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D ȣ���\n");
}

void SpineScript::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D ȣ���\n");
}

void SpineScript::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D ȣ���" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D ȣ���\n");
}

void SpineScript::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�
	if (Input::IsKeyPressed(VK_SPACE)) {
		spineRenderer->SetNextAnimation();
	}
	else if (Input::IsKeyPressed(VK_BACK)) {
		spineRenderer->SetPreviousAnimation();
	}
	else if (Input::IsKeyPressed('1')) {
		spineRenderer->SetAnimation("Idle");
	}
	else if (Input::IsKeyPressed('2')) {
		spineRenderer->SetAnimation("Move");
	}
	else if (Input::IsKeyPressed('3')) {
		spineRenderer->SetAnimation("Attack_Melee");
	}
	else if (Input::IsKeyPressed('4')) {
		SceneManager::GetInstance().ChangeScene(L"TitleScene");
	}
}
