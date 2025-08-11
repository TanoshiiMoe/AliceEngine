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
#include <Component/ParticleComponent.h>

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

    // 간단한 안내 텍스트 UI
    if (auto ui = NewObject<gameObject>(L"EffectUI"))
    {
        auto* txt = ui->AddComponent<TextRenderComponent>();
        txt->SetText(
            L"=== Particle Effect Test Scene ===\n\n"
            L"[A] Color Transform Effect (색상 변환)\n"
            L"[S] Distortion Effect (왜곡 효과)\n"
            L"[D] Blending Effect (블렌딩 효과)\n"
            L"[F] Physics Effect (물리 시뮬레이션)\n"
            L"[G] Environment Effect (환경 효과)\n"
            L"[H] Special Effect (특수 효과)\n\n"
            L"[SPACE] Clear All Effects\n"
            L"[1-5] Other Effects (Vignette, BlackOut)\n"
			L"[A] Color Transform Effect (색상 변환)\n"
			L"[S] Distortion Effect (왜곡 효과)\n"
			L"[D] Blending Effect (블렌딩 효과)\n"
			L"[F] Physics Effect (물리 시뮬레이션)\n"
			L"[G] Environment Effect (환경 효과)\n"
			L"[H] Special Effect (특수 효과)\n\n"
			L"[SPACE] Clear All Effects\n"
			L"[1-5] Other Effects (Vignette, BlackOut)\n"
        );
        txt->SetFontSize(18.0f);
        txt->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02f, 0.05f)));
    }

    // 파티클 효과 상세 설명 UI
    if (auto detailUI = NewObject<gameObject>(L"DetailUI"))
    {
        auto* detailTxt = detailUI->AddComponent<TextRenderComponent>();
        detailTxt->SetText(
            L"=== Effect Details ===\n\n"
            L"Color Transform: 동적 색상 변화, 노이즈, 파동 효과\n"
            L"Distortion: 물결, 소용돌이, 확장/수축 왜곡\n"
            L"Blending: 다중 레이어 블렌딩, 글로우, 블룸\n"
            L"Physics: 중력, 점성, 표면장력, 압력 시뮬레이션\n"
            L"Environment: 날씨, 조명, 반사/굴절 효과\n"
            L"Special: 마법, 폭발, 전기, 포털 특수 효과\n\n"
            L"각 키를 눌러서 해당 효과를 테스트하세요!"
        );
        detailTxt->SetFontSize(16.0f);
        detailTxt->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02f, 0.45f)));
    }

    // 파티클 효과들 생성
    CreateParticleEffects();
}

void EffectTestScene::CreateParticleEffects()
{
    // 1. 색상 변환 효과 (A키)
    if (auto* particle = NewObject<gameObject>(L"ColorTransformParticle"))
    {
        colorTransformEffect = particle->AddComponent<ParticleComponent>();
        colorTransformEffect->SetParticleType(EParticleType::BulletHit);
        colorTransformEffect->SetTexture(L"Mushroom.png");
        colorTransformEffect->SetEmissionPosition(FVector2(300, 300));
        colorTransformEffect->SetShaderType(EShaderType::ColorTransform);
        colorTransformEffect->SetShaderEffect(true, 2.0f);
        colorTransformEffect->SetPhysicsSimulation(true, 100.0f, 0.1f);
        colorTransformEffect->SetEmissionArea(FVector2(50, 50));
        colorTransformEffect->Play();
    }

    // 2. 왜곡 효과 (S키)
    if (auto* particle = NewObject<gameObject>(L"DistortionParticle"))
    {
        distortionEffect = particle->AddComponent<ParticleComponent>();
        distortionEffect->SetParticleType(EParticleType::Magic);
        distortionEffect->SetTexture(L"Mushroom.png");
        distortionEffect->SetEmissionPosition(FVector2(500, 300));
        distortionEffect->SetShaderType(EShaderType::Distortion);
        distortionEffect->SetShaderEffect(true, 1.5f);
        distortionEffect->SetPhysicsSimulation(true, 50.0f, 0.05f);
        distortionEffect->SetEmissionArea(FVector2(60, 60));
        distortionEffect->Play();
    }

    // 3. 블렌딩 효과 (D키)
    if (auto* particle = NewObject<gameObject>(L"BlendingParticle"))
    {
        blendingEffect = particle->AddComponent<ParticleComponent>();
        blendingEffect->SetParticleType(EParticleType::Explosion);
        blendingEffect->SetTexture(L"Mushroom.png");
        blendingEffect->SetEmissionPosition(FVector2(700, 300));
        blendingEffect->SetShaderType(EShaderType::Blending);
        blendingEffect->SetShaderEffect(true, 2.5f);
        blendingEffect->SetPhysicsSimulation(true, 150.0f, 0.15f);
        blendingEffect->SetEmissionArea(FVector2(70, 70));
        blendingEffect->Play();
    }

    // 4. 물리 시뮬레이션 효과 (F키)
    if (auto* particle = NewObject<gameObject>(L"PhysicsParticle"))
    {
        physicsEffect = particle->AddComponent<ParticleComponent>();
        physicsEffect->SetParticleType(EParticleType::Smoke);
        physicsEffect->SetTexture(L"Mushroom.png");
        physicsEffect->SetEmissionPosition(FVector2(300, 500));
        physicsEffect->SetShaderType(EShaderType::Physics);
        physicsEffect->SetShaderEffect(true, 1.8f);
        physicsEffect->SetPhysicsSimulation(true, 200.0f, 0.2f);
        physicsEffect->SetSurfaceTension(1.0f);
        physicsEffect->SetPressure(3.0f);
        physicsEffect->SetEmissionArea(FVector2(80, 80));
        physicsEffect->Play();
    }

    // 5. 환경 효과 (G키)
    if (auto* particle = NewObject<gameObject>(L"EnvironmentParticle"))
    {
        environmentEffect = particle->AddComponent<ParticleComponent>();
        environmentEffect->SetParticleType(EParticleType::Fire);
        environmentEffect->SetTexture(L"Mushroom.png");
        environmentEffect->SetEmissionPosition(FVector2(500, 500));
        environmentEffect->SetShaderType(EShaderType::Environment);
        environmentEffect->SetShaderEffect(true, 2.2f);
        environmentEffect->SetEnvironmentEffects(true, 0.7f, 0.8f);
        environmentEffect->SetReflectionRefraction(0.6f, 0.4f);
        environmentEffect->SetEmissionArea(FVector2(90, 90));
        environmentEffect->Play();
    }

    // 6. 특수 효과 (H키)
    if (auto* particle = NewObject<gameObject>(L"SpecialParticle"))
    {
        specialEffect = particle->AddComponent<ParticleComponent>();
        specialEffect->SetParticleType(EParticleType::PowerUp);
        specialEffect->SetTexture(L"Mushroom.png");
        specialEffect->SetEmissionPosition(FVector2(700, 500));
        specialEffect->SetShaderType(EShaderType::SpecialEffects);
        specialEffect->SetShaderEffect(true, 3.0f);
        specialEffect->SetSpecialEffects(true, 2.0f, 1.5f, 1.8f, 1.2f);
        specialEffect->SetEmissionArea(FVector2(100, 100));
        specialEffect->Play();
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
        
        // 파티클 효과들도 재시작
        if (colorTransformEffect) colorTransformEffect->Play();
        if (distortionEffect) distortionEffect->Play();
        if (blendingEffect) blendingEffect->Play();
        if (physicsEffect) physicsEffect->Play();
        if (environmentEffect) environmentEffect->Play();
        if (specialEffect) specialEffect->Play();
    }
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


