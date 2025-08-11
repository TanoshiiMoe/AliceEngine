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
#include "../Bike/BikeMovementScript.h"
#include <Helpers/StringHelper.h>
#include <Component/SpriteRenderer.h>
#include <GameManager/GamePlayManager.h>

PlayerManager* PlayerManager::instance = nullptr;

void PlayerManager::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void PlayerManager::OnStart()
{
	__super::Initialize();

	// instance 설정
	if (instance == nullptr)
		instance = this;
	else
		SceneManager::GetInstance().GetWorld()->RemoveObject(this->owner.lock());

	// SkewTransform 설정
	if (auto st = owner->GetComponent<SkewTransform>())
		st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");

	// SRT 설정
	owner->transform()->SetPosition(0, 0);
	owner->transform()->SetRotation(0);
	owner->transform()->SetScale(0.7f, 0.7f);
	owner->transform()->SetPivot(0.5f);

	// 애니메이터 설정
	AnimatorController::LoadAnimatorController(L"Player/KillDong/killdong_idle_AnimController.json", animController);
	animInstance = owner->GetComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Player/KillDong/killdong_idle_sprites.json");
	animInstance->LoadAnimationClip(L"Player/KillDong/killdong_idle_anim.json");
	animInstance->ApplyClipDurationsToStates();
	animInstance->Play();
	animInstance->OnStart();

	// 콜라이더 설정
	owner->GetComponent<Collider>()->SetBoxSize(FVector2(35, 10));
	if (auto collider = owner->GetComponent<Collider>())
	{
		collider->SetLayer(0);
		collider->boxComponent->SetRelativePosition(FVector2(0, -20));
	}

	// 인풋 컴포넌트
	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });
}

void PlayerManager::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	float playerDeltaSeconds = deltaSeconds * playerTimeScale;

	if (Input::IsKeyDown(VK_S))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if(st && st->zPos > minZ)
			st->zPos -= 100.0f * playerDeltaSeconds;
	}
	if (Input::IsKeyDown(VK_W))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if (st && st->zPos < maxZ)
			st->zPos += 100.0f * playerDeltaSeconds;
	}

	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}

	// 페이드 아웃 진행 중이면 투명도 감소
	if (m_isFading && m_fadeTargetSR)
	{
		m_fadeElapsed += deltaSeconds;
		float t = m_fadeElapsed / m_fadeDuration;
		if (t > 1.0f) t = 1.0f;
		m_fadeTargetSR->SetOpacity(1.0f - t);
	}
}

void PlayerManager::SetZClamp(float _min, float _max)
{
	_min = minZ;
	_max = maxZ;
}
 
void PlayerManager::Input()
{
	// 여기에 Input에 대한 로직 작성

	if (Input::IsKeyDown(VK_8))
	{
		owner->RemoveComponent<Collider>(owner->GetComponent<Collider>());
	}
	if (Input::IsKeyDown(VK_9))
	{
		if (owner->GetComponent<Collider>() == nullptr)
			owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 60));
	}


	// 산데비스탄 테스트
	if (Input::IsKeyPressed(VK_G)) {
		if (auto prism = owner->GetComponent<Prism>())
		{
			prism->SetActive(!prism->IsActive());
			TimerManager::GetInstance().SetGlobalTimeScale(0.5);
			playerTimeScale = 2.0f;
		}
	}
}

// Car::DelayDestroy 를 참고한 플레이어 지연 파괴
void PlayerManager::DelayDestroy()
{
    if (m_isDelaying) return;
    m_isDelaying = true;

    // 애니메이터/스프라이트 렌더러 가져오기
    AnimatorInstance* animator = owner->GetComponent<AnimatorInstance>();
    SpriteRenderer* srSelf = owner->GetComponent<SpriteRenderer>();

    // 고스트를 만들어 페이드 아웃 시킬 준비
    WeakObjectPtr<gameObject> ghostObj;
    SpriteRenderer* ghostSR = nullptr;

    if (animator && animator->sheet)
    {
        auto* clip = animator->animationClips[animator->curAnimationClip].get();
        if (clip && animator->m_curFrame < clip->frames.size())
        {
            FrameInfo* frameInfo = &(clip->frames[animator->m_curFrame]);
            int spriteIndex = animator->sheet->spriteIndexMap[frameInfo->sprite];
            SpriteInfo& sp = animator->sheet->sprites[spriteIndex];

            if (WeakObjectPtr<gameObject> temp = SceneManager::GetInstance().m_currentScene->NewObject<gameObject>(L"Player_Ghost"))
            {
                ghostObj = temp;
                ghostSR = temp->AddComponent<SpriteRenderer>();
                std::wstring textureRelPath = StringHelper::string_to_wstring(animator->sheet->texture);
                ghostSR->LoadData(textureRelPath);
                ghostSR->m_layer = srSelf ? srSelf->m_layer : animator->m_layer;

                temp->transform()->SetPosition(owner->transform()->GetPosition());
                temp->transform()->SetRotation(owner->transform()->GetRotation());
                temp->transform()->SetScale(animator->GetRelativeScale());
                ghostSR->SetSlice(sp.x, sp.y - sp.height, sp.width, sp.height);
                ghostSR->SetOpacity(1.0f);
                m_ghostGO = temp.Get();
            }
        }
    }

    // 더 이상 원본 애니메이션이 렌더되지 않도록 정지
    if (animator)
    {
        animator->Stop();
        animator->SetVisible(false);
    }
    if (srSelf)
    {
        srSelf->SetOpacity(0.0f);
    }

    // 1초 페이드아웃 설정
    m_skipGhostOnDestroy = true;
    m_fadeElapsed = 0.0f;
    m_fadeDuration = 1.0f;
    m_isFading = true;
    m_fadeTargetSR = ghostSR ? ghostSR : srSelf;

    TimerManager::GetInstance().ClearTimer(m_fadeHandle);
    TimerManager::GetInstance().SetTimer(
        m_fadeHandle,
        [weak = WeakFromThis<PlayerManager>(), ghostObj]() mutable {
            if (!weak) return;

			GamePlayManager::GetInstance().GameOver();
            //if (ghostObj)
            //{
            //    SceneManager::GetInstance().GetWorld()->RemoveObject(ghostObj.Get());
            //}
            //if (weak)
            //{
            //    weak->GetWorld()->RemoveObject(weak->GetOwner());
            //}
        },
        1.0f,
        false,
        1.0f);
}
