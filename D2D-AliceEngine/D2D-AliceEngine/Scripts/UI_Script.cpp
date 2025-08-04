#include "UI_Script.h"
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

void UI_Script::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void UI_Script::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void UI_Script::Awake()
{
}

void UI_Script::OnStart()
{
	m_owner = GetOwner();
	m_owner->AddComponent<UIImage>()->LoadData(L"Mushroom.png");
	m_owner->GetComponent<UIImage>()->SetRelativeScale(150);
	m_owner->GetComponent<UIImage>()->SetPosition(FVector2(50, 50));
	m_owner->GetComponent<UIImage>()->m_layer = 500;
	m_owner->GetComponent<UIImage>()->SetAction([]()
		{ 
		OutputDebugStringW(L"SetAction click!\n"); 
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		});

	m_owner->GetComponent<UIImage>();

	m_owner->AddComponent<UIText>()->SetText(L"Test");
	m_owner->GetComponent<UIText>()->SetFontSize(18.0f);
	m_owner->GetComponent<UIText>()->SetColor(FColor(0, 0, 0, 255));
	m_owner->GetComponent<UIText>()->SetPosition(FVector2(50, 300));
	m_owner->GetComponent<UIText>()->m_layer = 500;

	/*Canvas* canvas = GetWorld()->CreateCanvas<Canvas>(L"Canvas");
	canvas->transform()->SetPosition(50, 50);
	UIImage* image = canvas->AddUI<UIImage>();
	canvas->AddToChildUI(image);
	image->LoadData(L"Mushroom.png");
	image->SetScale(1);
	image->SetPosition(FVector2(-50, -50));
	image->m_layer = 200;

	UIText* text = canvas->AddUI<UIText>();
	canvas->AddToChildUI(text);
	text->SetText(L"Testsfgregdgdxhdfth");
	text->SetFontSize(20.0f);
	text->SetColor(FColor(0, 0, 0, 255));
	text->SetPosition(FVector2(0, 50));
	text->SetTextPosition(FVector2(0,0));
	text->m_layer = 200;*/
}

void UI_Script::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void UI_Script::OnDestroy()
{

}
