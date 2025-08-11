#include "GameOverScene.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/CoordHelper.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Widget/GameOverWidgetScript.h>

void GameOverScene::Initialize()
{
    __super::Initialize();
}

void GameOverScene::Release()
{
    __super::Release();
}

void GameOverScene::Update()
{
    __super::Update();
}

void GameOverScene::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"GameOverLabel");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();
    text->SetText(L"<현재 씬> GameOverScene");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(32.0f);

    // 중앙 클라우드 이미지
    if (auto* cloudObj = NewObject<gameObject>(L"Cloud"))
    {
        auto* sr = cloudObj->AddComponent<SpriteRenderer>();
        sr->SetDrawType(Define::EDrawType::ScreenSpace);
        sr->LoadData(L"cloud.jpg");
        // 중앙 배치
        sr->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));
        // 기본 비율 유지 (원본 사이즈)
    }

    m_widget = NewObject<gameObject>(L"Widget");
    m_widget->AddComponent<GameOverWidgetScript>();

    // VK_3 눌러 TitleScene 복귀
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(Define::Scene_Title);
        }
    });
}

void GameOverScene::OnExit()
{
    __super::OnExit();
    GamePlayManager::GetInstance().ReleaseTimers();
}
