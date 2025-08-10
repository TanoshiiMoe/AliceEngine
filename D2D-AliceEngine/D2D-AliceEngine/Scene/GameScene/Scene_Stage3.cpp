#include "Scene_Stage3.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>

void Scene_Stage3::Initialize()
{
    __super::Initialize();
}

void Scene_Stage3::Release()
{
    __super::Release();
}

void Scene_Stage3::Update()
{
    __super::Update();
}

void Scene_Stage3::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"Stage3Label");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();
    text->SetText(L"<���� ��> Scene_Stage3");
    text->SetTextAlignment(ETextFormat::TopLeft);
    text->SetRelativePosition(FVector2(20, 10));
    text->SetFontSize(24.0f);

    // VK_3 ������ TitleScene���� ��ȯ
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(L"TitleScene");
        }
    });
}

void Scene_Stage3::OnExit()
{
    __super::OnExit();
}


