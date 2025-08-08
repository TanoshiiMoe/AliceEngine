#include "PlayerManager.h"
#include "System/ScriptSystem.h"
#include "Component/SkewTransform.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimationController.h"
#include "Component/Collider.h"
#include "Component/BoxComponent.h"
#include <Core/Input.h>
#include "Effect/Prism.h"
#include "Manager/TimerManager.h"
#include <Component/InputComponent.h>
#include "Manager/UpdateTaskManager.h"

void PlayerManager::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void PlayerManager::OnStart()
{
	__super::Initialize();

	// SkewTransform ����
	if (auto st = owner->GetComponent<SkewTransform>())
		st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");

	// SRT ����
	owner->transform()->SetPosition(0, 0);
	owner->transform()->SetRotation(0);
	owner->transform()->SetScale(1.5f, 1.5f);
	owner->transform()->SetPivot(0.5f);

	// �ִϸ����� ����
	AnimatorController::LoadAnimatorController(L"Zero/Zero_AnimController.json", animController);
	animInstance = owner->GetComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Zero\\Zero_sprites.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_idle_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_attack_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_walk_anim.json");
	animInstance->ApplyClipDurationsToStates();
	animInstance->Play();
	animInstance->m_layer = 5;
	animInstance->OnStart();

	// �ݶ��̴� ����
	owner->GetComponent<Collider>()->SetBoxSize(FVector2(35, 10));
	if (auto collider = owner->GetComponent<Collider>())
	{
		collider->SetLayer(0);
		collider->boxComponent->SetRelativePosition(FVector2(0, -20));
	}

	// ��ǲ ������Ʈ
	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });
}

void PlayerManager::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

	float playerDeltaSeconds = deltaSeconds * playerTimeScale;

	//float speed = 125.0f;
	if (!(Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_DOWN) || Input::IsKeyDown(VK_UP)))
	{
		animInstance->SetFlip(true);
		animInstance->SetFloat("speed", 5.0f);
	}
	if (Input::IsKeyDown(VK_S))
	{
		owner->GetComponent<SkewTransform>()->zPos -= 100.0f * deltaSeconds;
	}
	if (Input::IsKeyDown(VK_W))
	{
		owner->GetComponent<SkewTransform>()->zPos += 100.0f * deltaSeconds;
	}

	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
}

void PlayerManager::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�

	if (Input::IsKeyDown(VK_8))
	{
		owner->RemoveComponent<Collider>(owner->GetComponent<Collider>());
	}
	if (Input::IsKeyDown(VK_9))
	{
		if (owner->GetComponent<Collider>() == nullptr)
			owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 60));
	}

	if (Input::IsKeyDown(VK_C))
	{
		//m_owner->stateMachine->SetNextState(L"Idle");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		animInstance->SetBool("attack", true);
	}
	else
	{
		animInstance->SetBool("attack", false);
	}

	// �굥��ź �׽�Ʈ
	if (Input::IsKeyPressed(VK_G)) {
		if (auto prism = owner->GetComponent<Prism>())
		{
			prism->SetActive(!prism->IsActive());
			TimerManager::GetInstance().SetGlobalTimeScale(0.5);
			playerTimeScale = 2.0f;
		}
	}
}
