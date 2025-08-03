#include "TitleWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>

void TitleWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void TitleWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void TitleWidgetScript::Awake()
{
}

void TitleWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->transform()->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f,0.5f)));
	auto text = m_owner->AddComponent<TextRenderComponent>();
	auto title = m_owner->AddComponent<TextRenderComponent>();
	auto button = m_owner->AddComponent<ButtonComponent>();
	auto background = m_owner->AddComponent<SpriteRenderer>();
	if (!text || !button) return;
	
	// ======================== button
	button->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	button->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	button->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	button->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 buttonSize = button->GetRelativeSize();
	button->SetRelativePosition(CoordHelper::RatioCoordToScreen(buttonSize, FVector2(0, -0.5)));
	button->SetRelativeScale(FVector2(1, 1));
	button->SetRelativeRotation(30);
	button->m_layer = 500;

	// ======================== title
	title->SetText(L"Button 1");
	title->SetFontSize(120.0f);
	title->SetColor(FColor(255, 0, 0, 255));
	FVector2 titleRectSize = title->GetRelativeSize();
	title->SetRelativePosition(CoordHelper::RatioCoordToScreen(titleRectSize, FVector2(-0.5, -0.5)) + FVector2(0, -300));
	title->SetRelativeScale(FVector2(1, 1));
	title->SetRelativeRotation(0);
	title->m_layer = 501;

	// ======================== text
	text->SetText(L"Button 1");
	text->SetFontSize(60.0f);
	text->SetColor(FColor(255, 0, 0, 255));
	FVector2 textRectSize = text->GetRelativeSize();
	text->SetRelativePosition(CoordHelper::RatioCoordToScreen(textRectSize, FVector2(-0.5, -0.5)));
	text->SetRelativeScale(FVector2(1, 1));
	text->SetRelativeRotation(0);
	text->m_layer = 501;
	text->RemoveFromParent();
	button->AddChildComponent(text);

	// ======================== background
	background->LoadData(L"tree.jpg");
	background->SetDrawType(Define::EDrawType::ScreenSpace);
	FVector2 backgroundSize = background->GetRelativeSize();
	background->SetRelativePosition(CoordHelper::RatioCoordToScreen(backgroundSize, FVector2(0, 0)));
	background->SetRelativeScale(FVector2(1, 1));
	background->SetRelativeRotation(0);

	// ======================== Delegete
	button->SetStateAction(Define::EButtonState::Pressed, []()
	{

		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		SceneManager::ChangeScene(L"HiroScene");
	});
}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void TitleWidgetScript::OnDestroy()
{

}
