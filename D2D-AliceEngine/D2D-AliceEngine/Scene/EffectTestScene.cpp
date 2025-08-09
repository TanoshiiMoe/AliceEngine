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
            L"Effect Test Scene\n"
            L"[1] Vignette (2.0s)\n"
            L"[2] BlackOut - VignetteOnly (2.0s)\n"
            L"[3] BlackOut - PointDiffuse (2.0s)\n"
            L"[4] BlackOut - SpotDiffuse (2.0s)\n"
            L"[5] BlackOut - SpotSpecular (2.0s)\n"
            L"[SPACE] Clear All\n"
        );
        txt->SetFontSize(20.0f);
        txt->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02f, 0.05f)));
    }
}

void EffectTestScene::OnExit()
{
    __super::OnExit();
}

void EffectTestScene::HandleInput()
{
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


