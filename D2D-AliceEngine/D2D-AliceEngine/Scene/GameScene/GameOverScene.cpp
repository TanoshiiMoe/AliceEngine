#include "GameOverScene.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/CoordHelper.h>

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
    text->SetText(L"<���� ��> GameOverScene");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(32.0f);

    // �߾� Ŭ���� �̹���
    if (auto* cloudObj = NewObject<gameObject>(L"Cloud"))
    {
        auto* sr = cloudObj->AddComponent<SpriteRenderer>();
        sr->SetDrawType(Define::EDrawType::ScreenSpace);
        sr->LoadData(L"cloud.jpg");
        // �߾� ��ġ
        sr->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));
        // �⺻ ���� ���� (���� ������)
    }

    // VK_3 ���� TitleScene ����
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
}
