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
#include <Helpers/CoordHelper.h>
#include <Component/ButtonComponent.h>

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

	if (!m_accel)
		return;

	m_accelTimer += deltaSeconds;
	m_realTimer += deltaSeconds;
	// 0.5초마다 점 개수 변경
	if (m_accelTimer >= 0.5f)
	{
		m_accelTimer = 0.0f;
		m_dotCount = (m_dotCount + 1) % 3;  // 0,1,2 반복
	}

	// 점(.) 문자열 만들기
	std::wstring dots(m_dotCount, L'.');

	m_accel->SetText(L"가속준비중 ." + dots);

	m_RealTime->SetText(m_realTimer);
}

void UI_Script::Awake()
{
}

void UI_Script::OnStart()
{
	//m_owner = GetOwner();
	//m_owner->AddComponent<UIImage>()->LoadData(L"Mushroom.png");
	//m_owner->GetComponent<UIImage>()->SetRelativeScale(150);
	//m_owner->GetComponent<UIImage>()->SetPosition(FVector2(50, 50));
	//m_owner->GetComponent<UIImage>()->m_layer = 500;
	//m_owner->GetComponent<UIImage>()->SetAction([]()
	//	{ 
	//	OutputDebugStringW(L"SetAction click!\n"); 
	//	OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
	//	});

	//m_owner->GetComponent<UIImage>();

	//m_owner->AddComponent<UIText>()->SetText(L"Test");
	//m_owner->GetComponent<UIText>()->SetFontSize(18.0f);
	//m_owner->GetComponent<UIText>()->SetColor(FColor(0, 0, 0, 255));
	//m_owner->GetComponent<UIText>()->SetPosition(FVector2(50, 300));
	//m_owner->GetComponent<UIText>()->m_layer = 540;

	m_UI_HUD = GetWorld()->NewObject<gameObject>(L"HUD");
	auto HUD = m_UI_HUD->AddComponent<SpriteRenderer>();
	GetCamera()->AddChildObject(m_UI_HUD);
	HUD->LoadData(L"UI\\UI_Time.png");
	HUD->SetDrawType(EDrawType::ScreenSpace);
	FVector2 HUDSize = HUD->GetRelativeSize();
	HUD->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(HUDSize, FVector2(0.5, 0.5))
		+ FVector2(30, 30));
	HUD->m_layer = 50;

	m_UI_PlayerHP = GetWorld()->NewObject<gameObject>(L"HP");
	GetCamera()->AddChildObject(m_UI_PlayerHP);
	auto HP = m_UI_PlayerHP->AddComponent<SpriteRenderer>();
	HP->LoadData(L"UI\\UI_1_HP.png");
	HP->SetDrawType(EDrawType::ScreenSpace);
	FVector2 HPSize = HP->GetRelativeSize();
	HP->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(HPSize, FVector2(0.5, 0.5))
		+ FVector2(30, 30));
	HP->m_layer = 51;

	m_UI_Dashboard = GetWorld()->NewObject<gameObject>(L"Dashboard");
	GetCamera()->AddChildObject(m_UI_Dashboard);
	auto Dashboard = m_UI_Dashboard->AddComponent<SpriteRenderer>();
	Dashboard->LoadData(L"UI\\UI_2_Speed+Battery_Background.png");
	Dashboard->SetDrawType(EDrawType::ScreenSpace);
	FVector2 BoadSize = Dashboard->GetRelativeSize();
	Dashboard->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(BoadSize, FVector2(0.5, 0.5))
		+ FVector2(0, 790));
	Dashboard->m_layer = 50;

	m_UI_BoardText = GetWorld()->NewObject<gameObject>(L"BoardText");
	GetCamera()->AddChildObject(m_UI_BoardText);
	auto BoardText = m_UI_BoardText->AddComponent<TextRenderComponent>();
	BoardText->SetFontSize(30.0);
	BoardText->SetColor(FColor(0, 234, 255, 255));
	BoardText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	BoardText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	BoardText->SetDrawType(EDrawType::ScreenSpace);
	FVector2 BoardTextSize = BoardText->GetRelativeSize();
	BoardText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(BoardTextSize, FVector2(0.5, 0.5))
		+ FVector2(270, 860));
	m_accel = BoardText;

	m_UI_currSpeed = GetWorld()->NewObject<gameObject>(L"Speed");
	GetCamera()->AddChildObject(m_UI_currSpeed);
	auto Speed = m_UI_currSpeed->AddComponent<SpriteRenderer>();
	Speed->LoadData(L"UI\\UI_2_Speed.png");
	Speed->SetDrawType(EDrawType::ScreenSpace);
	FVector2 SpeedSize = Speed->GetRelativeSize();
	Speed->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(SpeedSize, FVector2(0.5,0.5))
	+ FVector2(0, 790));
	Speed->m_layer = 51;

	m_UI_SpeedText = GetWorld()->NewObject<gameObject>(L"SpeedText");
	GetCamera()->AddChildObject(m_UI_SpeedText);
	auto SpeedText = m_UI_SpeedText->AddComponent<TextRenderComponent>();
	SpeedText->SetText(L"");
	SpeedText->SetFontSize(60.0);
	SpeedText->SetColor(FColor(0, 234, 255, 255));
	SpeedText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	SpeedText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	SpeedText->SetDrawType(EDrawType::ScreenSpace);
	SpeedText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(BoadSize, FVector2(0.5, 0.5))
		+ FVector2(0, 0));
	m_RealTime = SpeedText;

	m_UI_Battery = GetWorld()->NewObject<gameObject>(L"Battery");
	GetCamera()->AddChildObject(m_UI_Battery);
	auto Battery = m_UI_Battery->AddComponent<SpriteRenderer>();
	Battery->LoadData(L"UI\\UI_2_Battery.png");
	Battery->SetDrawType(EDrawType::ScreenSpace);
	FVector2 BatterySize = Battery->GetRelativeSize();
	Battery->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(BatterySize,FVector2(0.5, 0.5))
		+ FVector2(0, 790));
	Battery->m_layer = 51;

	// Button
	m_UI_Pause = GetWorld()->NewObject<gameObject>(L"Pause");
	GetCamera()->AddChildObject(m_UI_Pause);
	auto Pause = m_UI_Pause->AddComponent<ButtonComponent>();
	Pause->LoadData(Define::EButtonState::Idle, L"UI\\UI_Pause.png");
	Pause->LoadData(Define::EButtonState::Hover, L"UI\\UI_Pause.png");
	Pause->LoadData(Define::EButtonState::Pressed, L"UI\\UI_Pause.png");
	Pause->LoadData(Define::EButtonState::Release, L"UI\\UI_Pause.png");
	Pause->SetDrawType(EDrawType::ScreenSpace);
	FVector2 PauseSize = Pause->GetRelativeSize();
	Pause->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(PauseSize, FVector2(1, 0.5))
		+ FVector2(1800, 30)
	);
	Pause->m_layer = 50;

	// ================== Delegate
	Pause->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
		});

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
