#include "EffectTestScene.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Helpers/CoordHelper.h>
#include <Scripts/Widget/VignetteWidgetScript.h>
#include <Scripts/Widget/BlackOutWidgetScript.h>
#include <Component/Effect/ParticleComponent.h>
#include <Component/Effect/ShaderParticleComponent.h>
#include <Scripts/Video/YuukaVideo.h>
#include <Component/SpriteRenderer.h>

void EffectTestScene::Initialize()
{
    __super::Initialize();
}

void EffectTestScene::Release()
{
    __super::Release();
}

void EffectTestScene::Update()
{
    __super::Update();
    HandleInput();
}

void EffectTestScene::OnEnter()
{
    __super::OnEnter();

    gameObject* m_yuuka = NewObject<gameObject>(L"yuuka");
    m_yuuka->AddComponent<SpriteRenderer>()->LoadData(L"tree.jpg");

    // 간단한 안내 텍스트 UI
    if (auto ui = NewObject<gameObject>(L"EffectUI"))
    {
        auto* txt = ui->AddComponent<TextRenderComponent>();
        txt->SetText(
            L"Effect Test Scene\n"
            L"[1] Vignette (2.0s)\n"
            L"[2] BlackOut - VignetteOnly (2.0s)\n"
            L"[3] BlackOut - PointDiffuse (2.0s)\n"
            L"[4] BlackOut - SpotDiffuse (2.0s)\n"
            L"[5] BlackOut - SpotSpecular (2.0s)\n"
            L"[A] Particle Explosion (월드)\n"
            L"[B] Particle Impact (월드)\n"
            L"[C] ClickBurst L (화면)\n"
            L"[D] ClickBurst R (화면)\n"
            L"[E] Toggle Mouse Trail (화면)\n"
            L"[F] Aura (월드)\n"
            L"[G] Electric (월드)\n"
            L"[H] Portal Swirl (월드)\n"
            L"[R] Shader Blending\n"
            L"[T] Shader ColorChange\n"
            L"[Y] Shader Distortion\n"
            L"[U] Shader Special\n"
            L"[I] Shader Environment\n"
            L"[O] Shader Physics\n"
            L"[SPACE] Clear All\n"
        );
        txt->SetFontSize(18.0f);
        txt->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02f, 0.05f)));
    }
}

void EffectTestScene::OnExit()
{
    __super::OnExit();
}

void EffectTestScene::HandleInput()
{
    // 파티클 효과 테스트
    if (Input::IsKeyPressed('A') || Input::IsKeyPressed('a'))
    {
        if (colorTransformEffect) colorTransformEffect->Play();
    }
    if (Input::IsKeyPressed('S') || Input::IsKeyPressed('s'))
    {
        if (distortionEffect) distortionEffect->Play();
    }
    if (Input::IsKeyPressed('D') || Input::IsKeyPressed('d'))
    {
        if (blendingEffect) blendingEffect->Play();
    }
    if (Input::IsKeyPressed('F') || Input::IsKeyPressed('f'))
    {
        if (physicsEffect) physicsEffect->Play();
    }
    if (Input::IsKeyPressed('G') || Input::IsKeyPressed('g'))
    {
        if (environmentEffect) environmentEffect->Play();
    }
    if (Input::IsKeyPressed('H') || Input::IsKeyPressed('h'))
    {
        if (specialEffect) specialEffect->Play();
    }

    // 기존 효과들
    if (Input::IsKeyPressed('1'))
    {
        SpawnVignette(2.0f, 1.0f);
    }
    if (Input::IsKeyPressed('2'))
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::VignetteOnly), true, 2.0f, 1.0f);
    }
    if (Input::IsKeyPressed('3'))
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::PointDiffuse), true, 2.0f, 1.0f);
    }
    if (Input::IsKeyPressed('4'))
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::SpotDiffuse), true, 2.0f, 1.0f);
    }
    if (Input::IsKeyPressed('5'))
    {
        SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::SpotSpecular), true, 2.0f, 1.0f);
    }
    if (Input::IsKeyPressed(VK_SPACE))
    {
        // 전부 제거
        GetWorld()->RemoveObjectByName(L"VignetteOverlay");
        GetWorld()->RemoveObjectByName(L"BlackOutOverlay");
        GetWorld()->RemoveObjectByName(L"ParticleWorld");
        GetWorld()->RemoveObjectByName(L"ParticleScreen");
    }

    // a~h 파티클 테스트
    if (Input::IsKeyPressed(VK_A)) SpawnParticleExplosion();
    if (Input::IsKeyPressed(VK_B)) SpawnParticleImpact();
    if (Input::IsKeyPressed(VK_C)) SpawnParticleClickL();
    if (Input::IsKeyPressed(VK_D)) SpawnParticleClickR();
    if (Input::IsKeyPressed(VK_E)) ToggleMouseTrail();
    if (Input::IsKeyPressed(VK_F)) SpawnParticleAura();
    if (Input::IsKeyPressed(VK_G)) SpawnParticleElectric();
    if (Input::IsKeyPressed(VK_H)) SpawnParticlePortal();

    // Shader 기반 효과 미리보기
    /*if (Input::IsKeyPressed(VK_R)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::Blending);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }
    if (Input::IsKeyPressed(VK_T)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::ColorChange);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }
    if (Input::IsKeyPressed(VK_Y)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::Distortion);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }
    if (Input::IsKeyPressed(VK_U)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::Special);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }
    if (Input::IsKeyPressed(VK_I)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::Environment);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }
    if (Input::IsKeyPressed(VK_O)) {
        auto go = GetWorld()->FindObjectByName<gameObject>(L"ShaderFX"); if (!go) go = NewObject<gameObject>(L"ShaderFX");
        auto* sp = go->GetComponent<ShaderParticleComponent>(); if (!sp) sp = go->AddComponent<ShaderParticleComponent>();
        sp->SetShader(ShaderParticleComponent::EShaderKind::Physics);
        sp->SetDrawType(Define::EDrawType::ScreenSpace);
        sp->SetAdditive(true);
        sp->SetSize(FVector2(640, 360));
    }*/
}

void EffectTestScene::SpawnVignette(float durationSec, float maxAlpha)
{
    if (auto go = NewObject<gameObject>(L"VignetteOverlay"))
    {
        auto* vig = go->AddComponent<VignetteWidgetScript>();
        vig->SetDuration(durationSec);
        vig->SetMaxEdgeAlpha(maxAlpha);
    }
}

void EffectTestScene::SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha)
{
    if (auto go = NewObject<gameObject>(L"BlackOutOverlay"))
    {
        auto* bo = go->AddComponent<BlackOutWidgetScript>();
        bo->SetDuration(durationSec);
        bo->SetMaxEdgeAlpha(maxAlpha);
        bo->SetMode(static_cast<EBlackOutLightingMode>(modeIndex));
        bo->UseCrossFade(useCrossFade);
    }
}

// =============== 파티클 테스트 스폰 구현 ===============
void EffectTestScene::SpawnParticleExplosion()
{
    // 월드 공간 파티클 객체 (없으면 만들고 재사용)
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleWorld");
    if (!go) go = NewObject<gameObject>(L"ParticleWorld");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::WorldSpace);
    pc->SetAdditiveBlend(true);
    //pc->EmitExplosion(Input::GetMouseWorldPosition());
    pc->EmitExplosion(Input::GetMouseWorldPosition());
}

void EffectTestScene::SpawnParticleImpact()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleWorld");
    if (!go) go = NewObject<gameObject>(L"ParticleWorld");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::WorldSpace);
    pc->SetAdditiveBlend(true);
    pc->EmitImpact(Input::GetMouseWorldPosition());
}

void EffectTestScene::SpawnParticleClickL()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleScreen");
    if (!go) go = NewObject<gameObject>(L"ParticleScreen");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::ScreenSpace);
    pc->SetAdditiveBlend(true);
    pc->EmitClickBurst(Input::GetMousePosition(), false);
}

void EffectTestScene::SpawnParticleClickR()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleScreen");
    if (!go) go = NewObject<gameObject>(L"ParticleScreen");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::ScreenSpace);
    pc->SetAdditiveBlend(true);
    pc->EmitClickBurst(Input::GetMousePosition(), true);
}

void EffectTestScene::ToggleMouseTrail()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleScreen");
    if (!go) go = NewObject<gameObject>(L"ParticleScreen");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::ScreenSpace);
    static bool on = false; on = !on;
    pc->ToggleMouseTrail(on);
}

void EffectTestScene::SpawnParticleAura()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleWorld");
    if (!go) go = NewObject<gameObject>(L"ParticleWorld");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::WorldSpace);
    pc->SetAdditiveBlend(true);
    // 화면 중앙 근처에 오라 생성
    FVector2 center = SceneManager::GetCamera()->GetPosition();
    pc->EmitAura(center, 100.0f);
}

void EffectTestScene::SpawnParticleElectric()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleWorld");
    if (!go) go = NewObject<gameObject>(L"ParticleWorld");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::WorldSpace);
    pc->SetAdditiveBlend(true);
    pc->EmitElectric(Input::GetMouseWorldPosition());
}

void EffectTestScene::SpawnParticlePortal()
{
    WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"ParticleWorld");
    if (!go) go = NewObject<gameObject>(L"ParticleWorld");
    auto* pc = go->GetComponent<ParticleComponent>();
    if (!pc) pc = go->AddComponent<ParticleComponent>();
    pc->SetDrawType(Define::EDrawType::WorldSpace);
    pc->SetAdditiveBlend(true);
    pc->EmitPortalSwirl(Input::GetMouseWorldPosition());
}


