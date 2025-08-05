#include "TitleWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>

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

	GetCamera()->AddChildObject(m_owner);

	auto background = m_owner->AddComponent<SpriteRenderer>();

	auto startText = m_owner->AddComponent<TextRenderComponent>();
	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto staffText = m_owner->AddComponent<TextRenderComponent>();
	auto quitText = m_owner->AddComponent<TextRenderComponent>();
	auto closeText = m_owner->AddComponent<TextRenderComponent>();

	auto mainTitle = m_owner->AddComponent<TextRenderComponent>();
	auto subTitle = m_owner->AddComponent<TextRenderComponent>();

	auto startButton = m_owner->AddComponent<ButtonComponent>();
	auto continueButton = m_owner->AddComponent<ButtonComponent>();
	auto optionButton = m_owner->AddComponent<ButtonComponent>();
	auto staffButton = m_owner->AddComponent<ButtonComponent>();
	auto quitButton = m_owner->AddComponent<ButtonComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	auto tutorial = m_owner->AddComponent<VideoComponent>();
	tutorial->LoadData(L"BackGround\\Mari_Sportswear.webm",30,L"jpg",95,true);
	tutorial->m_layer = -1000;
	tutorial->SetRelativePosition(FVector2(-960, -550));

	if (!startText || !startButton) return;
	if (!continueText || !continueButton) return;
	if (!optionText || !optionButton) return;
	if (!staffText || !staffButton) return;
	if (!quitText || !quitButton) return;
	if (!tutorial || !closeText || !closeButton) return;

	float buttonBasePos = 500;

	// ======================== startButton
	startButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	startButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	startButton->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 startButtonSize = startButton->GetRelativeSize();
	startButton->SetRelativePosition(CoordHelper::RatioCoordToScreen(startButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, -150));
	startButton->SetRelativeScale(FVector2(1, 1));
	startButton->SetRelativeRotation(0);
	startButton->m_layer = 300;

	// ======================== continueButton
	continueButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	continueButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	continueButton->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 continueButtonSize = continueButton->GetRelativeSize();
	continueButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 0));
	continueButton->SetRelativeScale(FVector2(1, 1));
	continueButton->SetRelativeRotation(0);
	continueButton->m_layer = 500;

	// ======================== optionButton
	optionButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	optionButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	optionButton->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 optionButtonSize = optionButton->GetRelativeSize();
	optionButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 150));
	optionButton->SetRelativeScale(FVector2(1, 1));
	optionButton->SetRelativeRotation(0);
	optionButton->m_layer = 500;

	// ======================== staffButton
	staffButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	staffButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	staffButton->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 staffButtonSize = staffButton->GetRelativeSize();
	staffButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 300));
	staffButton->SetRelativeScale(FVector2(1, 1));
	staffButton->SetRelativeRotation(0);
	staffButton->m_layer = 500;

	// ======================== quitButton
	quitButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	quitButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	quitButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	quitButton->LoadData(Define::EButtonState::Release, L"dead.png");
	FVector2 quitButtonSize = quitButton->GetRelativeSize();
	quitButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(quitButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 450));
	quitButton->SetRelativeScale(FVector2(1, 1));
	quitButton->SetRelativeRotation(0);
	quitButton->m_layer = 500;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"Button_Hover.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"Button_Pressed.png");
	closeButton->LoadData(Define::EButtonState::Release, L"dead.png");
	closeButton->SetRelativePosition(FVector2(0,350));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetRelativeRotation(0);
	closeButton->m_layer = -1000;

	// ======================== mainTitle
	mainTitle->SetText(L"높다락길의 질주");
	mainTitle->SetFontSize(120.0f);
	mainTitle->SetColor(FColor(255, 0, 0, 255));
	FVector2 mainTitleRectSize = mainTitle->GetRelativeSize();
	mainTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, -300));
	mainTitle->SetRelativeScale(FVector2(1, 1));
	mainTitle->SetRelativeRotation(0);
	mainTitle->m_layer = 501;

	// ======================== subTitle
	subTitle->SetText(L"~활빈은 두령을 쫒는다~");
	subTitle->SetFontSize(60.0f);
	subTitle->SetColor(FColor(255, 0, 0, 255));
	FVector2 subTitleRectSize = subTitle->GetRelativeSize();
	subTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(subTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, 0));
	subTitle->SetRelativeScale(FVector2(1, 1));
	subTitle->SetRelativeRotation(0);
	subTitle->m_layer = 501;

	// ======================== startText
	startText->SetText(L"시작하기");
	startText->SetFontSize(60.0f);
	startText->SetColor(FColor(255, 0, 0, 255));
	FVector2 startTextRectSize = startText->GetRelativeSize();
	startText->SetRelativePosition(CoordHelper::RatioCoordToScreen(startTextRectSize, FVector2(-0.5, -0.5)));
	startText->SetRelativeScale(FVector2(1, 1));
	startText->SetRelativeRotation(0);
	startText->m_layer = 501;
	startText->RemoveFromParent();
	startButton->AddChildComponent(startText);

	// ======================== continueText
	continueText->SetText(L"이어하기");
	continueText->SetFontSize(60.0f);
	continueText->SetColor(FColor(255, 0, 0, 255));
	FVector2 continueTextRectSize = continueText->GetRelativeSize();
	continueText->SetRelativePosition(CoordHelper::RatioCoordToScreen(continueTextRectSize, FVector2(-0.5, -0.5)));
	continueText->SetRelativeScale(FVector2(1, 1));
	continueText->SetRelativeRotation(0);
	continueText->m_layer = 501;
	continueText->RemoveFromParent();
	continueButton->AddChildComponent(continueText);

	// ======================== optionText
	optionText->SetText(L"음량 조정");
	optionText->SetFontSize(60.0f);
	optionText->SetColor(FColor(255, 0, 0, 255));
	FVector2 optionTextRectSize = optionText->GetRelativeSize();
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionTextRectSize, FVector2(-0.5, -0.5)));
	optionText->SetRelativeScale(FVector2(1, 1));
	optionText->SetRelativeRotation(0);
	optionText->m_layer = 501;
	optionText->RemoveFromParent();
	optionButton->AddChildComponent(optionText);

	// ======================== staffText
	staffText->SetText(L"제작진");
	staffText->SetFontSize(60.0f);
	staffText->SetColor(FColor(255, 0, 0, 255));
	FVector2 staffTextRectSize = continueText->GetRelativeSize();
	staffText->SetRelativePosition(CoordHelper::RatioCoordToScreen(staffTextRectSize, FVector2(-0.5, -0.5)));
	staffText->SetRelativeScale(FVector2(1, 1));
	staffText->SetRelativeRotation(0);
	staffText->m_layer = 501;
	staffText->RemoveFromParent();
	staffButton->AddChildComponent(staffText);

	// ======================== quitText
	quitText->SetText(L"오락 종료");
	quitText->SetFontSize(60.0f);
	quitText->SetColor(FColor(255, 0, 0, 255));
	FVector2 quitTextRectSize = quitText->GetRelativeSize();
	quitText->SetRelativePosition(CoordHelper::RatioCoordToScreen(quitTextRectSize, FVector2(-0.5, -0.5)));
	quitText->SetRelativeScale(FVector2(1, 1));
	quitText->SetRelativeRotation(0);
	quitText->m_layer = 501;
	quitText->RemoveFromParent();
	quitButton->AddChildComponent(quitText);

	// ======================== closeText
	closeText->SetText(L"닫기");
	closeText->SetFontSize(60.0f);
	closeText->SetColor(FColor(255, 0, 0, 255));
	FVector2 closeTextRectSize = closeText->GetRelativeSize();
	closeText->SetRelativePosition(CoordHelper::RatioCoordToScreen(closeTextRectSize, FVector2(-0.5, -0.5)));
	closeText->SetRelativeScale(FVector2(1, 1));
	closeText->SetRelativeRotation(0);
	closeText->m_layer = -1000;
	closeButton->SetActive(false);
	closeText->RemoveFromParent();
	closeButton->AddChildComponent(closeText);

	// ======================== background
	background->LoadData(L"tree.jpg");
	background->SetDrawType(Define::EDrawType::ScreenSpace);
	FVector2 backgroundSize = background->GetRelativeSize();
	background->SetRelativePosition(CoordHelper::RatioCoordToScreen(backgroundSize, FVector2(0, 0)));
	background->SetRelativeScale(FVector2(1, 1));
	background->SetRelativeRotation(0);

	// ======================== Delegete
	startButton->SetStateAction(Define::EButtonState::Pressed, [
		tutorial, startButton, quitButton, staffButton, optionButton, closeButton, closeText]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		//SceneManager::ChangeScene(L"HiroScene");

		startButton->SetActive(false);
		quitButton->SetActive(false);
		staffButton->SetActive(false);
		optionButton->SetActive(false);

		closeButton->m_layer = 503;
		closeButton->SetActive(true);
		closeText->m_layer = 504;

		tutorial->m_layer = 502;
		tutorial->Play();
	});

	continueButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			SceneManager::ChangeScene(L"HiroScene");
		});

	optionButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
		});

	staffButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
		});

	quitButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			// Quit
			PostQuitMessage(0);
		});
}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void TitleWidgetScript::OnDestroy()
{

}
