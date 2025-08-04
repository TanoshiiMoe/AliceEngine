#include "TitleUIScript.h"
#include <Scene/Scene.h>

#include <System/ScriptSystem.h>
#include <Math/Transform.h>

#include <Core/Input.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>

#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <UI/UIButton.h>

void TitleUIScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void TitleUIScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TitleUIScript::Awake()
{
}

void TitleUIScript::OnStart()
{
	m_owner = GetOwner();
	auto image = m_owner->AddComponent<UIImage>();
	auto mainTitle = m_owner->AddComponent<UIText>();
	auto subTitle = m_owner->AddComponent<UIText>();
	auto button = m_owner->AddComponent<UIButton>();

	if (!image ||
		!mainTitle ||
		!button ||
		!subTitle
		) return;

	//Anchor를 지정하면 Offset은 D2D좌표계를 사용합니다.

	image->LoadData(L"Mushroom.png");
	//image->SetScale(150);
	//image->SetPosition(FVector2(50, 50));
	image->SetAnchor(EUIScreenAnchor::MiddleCenter);
	image->m_layer = 500;

	// Delegete
	image->SetAction([]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		});

	// text
	mainTitle->SetText(L"높다락길의 질주");
	mainTitle->SetFontSize(60.0f);
	mainTitle->SetColor(FColor(0, 0, 0, 255));
	//text->SetPosition(FVector2(Define::SCREEN_WIDTH / 2, 40));
	mainTitle->SetAnchor(EUIScreenAnchor::MiddleRight);
	mainTitle->m_layer = 500;

	subTitle->SetText(L"~활빈은 두령을 쫒는다~");
	subTitle->SetFontSize(30.0f);
	subTitle->SetColor(FColor::Black);
	subTitle->SetAnchor(EUIScreenAnchor::MiddleRight, 0, 50);
	subTitle->m_layer = 500;

	button->SetImages(L"Button_Idle.png",L"Button_Hover.png", L"Button_Pressed.png");
	//button->SetScale(150);
	// 보류
	//button->SetHoverScale(160);
	button->SetAnchor(EUIScreenAnchor::MiddleCenter, 0, 250);
	//button->SetPosition(FVector2(100,100));

	button->SetAction([] {});
}

void TitleUIScript::OnEnd()
{
}

void TitleUIScript::OnDestroy()
{
}
