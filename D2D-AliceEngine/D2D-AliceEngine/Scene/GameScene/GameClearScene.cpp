#include "GameClearScene.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/CoordHelper.h>

void GameClearScene::Initialize()
{
    __super::Initialize();
}

void GameClearScene::Release()
{
    __super::Release();
}

void GameClearScene::Update()
{
    __super::Update();
}

void GameClearScene::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"GameClearLabel");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();
    text->SetText(L"<현재 씬> GameClearScene");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(32.0f);

    // 중앙 클라우드 이미지
    if (auto* cloudObj = NewObject<gameObject>(L"Cloud"))
    {
        auto* sr = cloudObj->AddComponent<SpriteRenderer>();
        sr->SetDrawType(Define::EDrawType::ScreenSpace);
        sr->LoadData(L"cloud.jpg");
        sr->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));
    }

    // VK_3 눌러 TitleScene 복귀
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(Define::Scene_Title);
        }
    });
}

void GameClearScene::OnExit()
{
    __super::OnExit();
}
