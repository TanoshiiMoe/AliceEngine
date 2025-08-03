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
	auto button = m_owner->AddComponent<ButtonComponent>();
	if (!text || !button) return;
	
	button->LoadData(L"Mushroom.png");
	FVector2 buttonSize = button->GetRelativeSize();
	button->SetRelativePosition(CoordHelper::RatioCoordToScreen(buttonSize, FVector2(0, -0.5)));
	button->SetRelativeScale(FVector2(1, 1));
	button->SetRelativeRotation(30);
	button->m_layer = 500;

	// Delegete
	button->SetAction([]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
	});

	text->SetText(L"Title");
	text->SetFontSize(60.0f);
	text->SetColor(FColor(0, 0, 0, 255));
	//text->SetPosition(FVector2(Define::SCREEN_WIDTH / 2, 40));
	FVector2 textRectSize = text->GetRelativeSize();
	text->SetRelativePosition(CoordHelper::RatioCoordToScreen(textRectSize, FVector2(-0.5, -0.5)));
	//text->SetTextAlignment(ETextFormat::TopLeft);
	text->m_layer = 500;

	//button->SetImages(L"Button_Idle.png", L"Button_Hover.png", L"Button_Pressed.png");
	//button->SetScale(150);
	// 보류
	//button->SetHoverScale(160);
	//button->SetAnchor(EUIScreenAnchor::MiddleCenter, 0, 250);
	//button->SetPosition(FVector2(100,100));

}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void TitleWidgetScript::OnDestroy()
{

}
