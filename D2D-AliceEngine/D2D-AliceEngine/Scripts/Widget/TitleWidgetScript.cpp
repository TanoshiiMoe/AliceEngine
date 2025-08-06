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
#include <Component/AudioComponent.h>

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
	auto startButton = m_owner->AddComponent<ButtonComponent>();

	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto continueButton = m_owner->AddComponent<ButtonComponent>();
	auto continueTabText = m_owner->AddComponent<TextRenderComponent>();

	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto optionButton = m_owner->AddComponent<ButtonComponent>();
	auto optionTabText = m_owner->AddComponent<TextRenderComponent>();

	auto staffText = m_owner->AddComponent<TextRenderComponent>();
	auto staffButton = m_owner->AddComponent<ButtonComponent>();
	auto staffTabText = m_owner->AddComponent<TextRenderComponent>();

	auto quitText = m_owner->AddComponent<TextRenderComponent>();
	auto quitButton = m_owner->AddComponent<ButtonComponent>();

	auto closeText = m_owner->AddComponent<TextRenderComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	auto mainTitle = m_owner->AddComponent<TextRenderComponent>();
	auto subTitle = m_owner->AddComponent<TextRenderComponent>();

	auto uiSound = m_owner->AddComponent<AudioComponent>();
	uiSound->Load(L"UI_interact_sound.wav", AudioMode::Memory);

	auto tutorial = m_owner->AddComponent<VideoComponent>();
	tutorial->LoadData(L"BackGround\\Mari_Sportswear.webm",30,L"jpg",95,true);
	tutorial->m_layer = -1000;
	tutorial->SetRelativePosition(FVector2(-960, -550));

	auto PopupTab = m_owner->AddComponent<SpriteRenderer>();
	PopupTab->LoadData(L"UI\\Continue.png");
	PopupTab->m_layer = -1000;
	PopupTab->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f , -SCREEN_HEIGHT / 2.0f));

	if (!startText || !startButton) return;
	if (!continueText || !continueButton) return;
	if (!optionText || !optionButton) return;
	if (!staffText || !staffButton) return;
	if (!quitText || !quitButton) return;
	if (!tutorial || !closeText || !closeButton) return;

	float buttonBasePos = 400;

	// ======================== startButton
	startButton->SetTag(L"Button");
	startButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 startButtonSize = startButton->GetRelativeSize();
	startButton->SetRelativePosition(CoordHelper::RatioCoordToScreen(startButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, -150));
	startButton->SetRelativeScale(FVector2(1, 1));
	startButton->SetRelativeRotation(0);
	startButton->m_layer = 300;

	// ======================== continueButton
	continueButton->SetTag(L"Button");
	continueButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 continueButtonSize = continueButton->GetRelativeSize();
	continueButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 0));
	continueButton->SetRelativeScale(FVector2(1, 1));
	continueButton->SetRelativeRotation(0);
	continueButton->m_layer = 500;

	// ======================== optionButton
	optionButton->SetTag(L"Button");
	optionButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 optionButtonSize = optionButton->GetRelativeSize();
	optionButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 150));
	optionButton->SetRelativeScale(FVector2(1, 1));
	optionButton->SetRelativeRotation(0);
	optionButton->m_layer = 500;

	// ======================== staffButton
	staffButton->SetTag(L"Button");
	staffButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 staffButtonSize = staffButton->GetRelativeSize();
	staffButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 300));
	staffButton->SetRelativeScale(FVector2(1, 1));
	staffButton->SetRelativeRotation(0);
	staffButton->m_layer = 500;

	// ======================== quitButton
	quitButton->SetTag(L"Button");
	quitButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	quitButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	quitButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	quitButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 quitButtonSize = quitButton->GetRelativeSize();
	quitButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(quitButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 450));
	quitButton->SetRelativeScale(FVector2(1, 1));
	quitButton->SetRelativeRotation(0);
	quitButton->m_layer = 500;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	closeButton->SetRelativePosition(FVector2(0,350));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetRelativeRotation(0);
	closeButton->m_layer = -1000;

	// ======================== mainTitle
	mainTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	mainTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	mainTitle->SetText(L"높다락길의 질주");
	mainTitle->SetFontSize(120.0f);
	mainTitle->SetColor(FColor(242, 207, 238, 255));
	FVector2 mainTitleRectSize = mainTitle->GetRelativeSize();
	mainTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, -300));
	mainTitle->SetRelativeScale(FVector2(1, 1));
	mainTitle->SetRelativeRotation(0);
	mainTitle->m_layer = 501;

	// ======================== subTitle
	subTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	subTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	subTitle->SetText(L"~활빈은 두령을 쫒는다~");
	subTitle->SetFontSize(60.0f);
	subTitle->SetColor(FColor(242, 207, 238, 255));
	FVector2 subTitleRectSize = subTitle->GetRelativeSize();
	subTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(subTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, 0));
	subTitle->SetRelativeScale(FVector2(1, 1));
	subTitle->SetRelativeRotation(0);
	subTitle->m_layer = 501;

	// ======================== startText
	startText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	startText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	startText->SetText(L"시작하기");
	startText->SetFontSize(55.0f);
	startText->SetColor(FColor::White);
	FVector2 startTextRectSize = startText->GetRelativeSize();
	startText->SetRelativePosition(CoordHelper::RatioCoordToScreen(startTextRectSize, FVector2(-0.5, -0.5)));
	startText->SetRelativeScale(FVector2(1, 1));
	startText->SetRelativeRotation(0);
	startText->m_layer = 501;
	startText->RemoveFromParent();
	startButton->AddChildComponent(startText);

	// ======================== continueText
	continueText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	continueText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	continueText->SetText(L"이어하기");
	continueText->SetFontSize(55.0f);
	continueText->SetColor(FColor::White);
	FVector2 continueTextRectSize = continueText->GetRelativeSize();
	continueText->SetRelativePosition(CoordHelper::RatioCoordToScreen(continueTextRectSize, FVector2(-0.5, -0.5)));
	continueText->SetRelativeScale(FVector2(1, 1));
	continueText->SetRelativeRotation(0);
	continueText->m_layer = 501;
	continueText->RemoveFromParent();
	continueButton->AddChildComponent(continueText);

	continueTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	continueTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	continueTabText->SetText(L"이어하기");
	continueTabText->SetFontSize(70.0f);
	continueTabText->SetColor(FColor(0, 234, 255, 255));
	FVector2 continueTabTextRectSize = continueTabText->GetRelativeSize();
	continueTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -350)
	);
	continueTabText->SetRelativeRotation(0);
	continueTabText->m_layer = -1000;

	// ======================== optionText
	optionText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionText->SetText(L"음량 조정");
	optionText->SetFontSize(55.0f);
	optionText->SetColor(FColor::White);
	FVector2 optionTextRectSize = optionText->GetRelativeSize();
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionTextRectSize, FVector2(-0.5, -0.5)));
	optionText->SetRelativeScale(FVector2(1, 1));
	optionText->SetRelativeRotation(0);
	optionText->m_layer = 501;
	optionText->RemoveFromParent();
	optionButton->AddChildComponent(optionText);

	optionTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabText->SetText(L"음량 조절");
	optionTabText->SetFontSize(70.0f);
	optionTabText->SetColor(FColor(0, 234, 255, 255));
	FVector2 optionTabTextRectSize = optionTabText->GetRelativeSize();
	optionTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -350)
	);
	optionTabText->SetRelativeRotation(0);
	optionTabText->m_layer = -1000;

	// ======================== staffText
	staffText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffText->SetText(L"제작진");
	staffText->SetFontSize(55.0f);
	staffText->SetColor(FColor::White);
	FVector2 staffTextRectSize = continueText->GetRelativeSize();
	staffText->SetRelativePosition(CoordHelper::RatioCoordToScreen(staffTextRectSize, FVector2(-0.4, -0.5)));
	staffText->SetRelativeScale(FVector2(1, 1));
	staffText->SetRelativeRotation(0);
	staffText->m_layer = 501;
	staffText->RemoveFromParent();
	staffButton->AddChildComponent(staffText);

	staffTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffTabText->SetText(L"제작진");
	staffTabText->SetFontSize(70.0f);
	staffTabText->SetColor(FColor(0, 234, 255, 255));
	FVector2 staffTabTextSize = staffTabText->GetRelativeSize();
	staffTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffTabTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -350)
	);
	staffTabText->SetRelativeRotation(0);
	staffTabText->m_layer = -1000;

	// ======================== quitText
	quitText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	quitText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	quitText->SetText(L"오락 종료");
	quitText->SetFontSize(55.0f);
	quitText->SetColor(FColor::White);
	FVector2 quitTextRectSize = quitText->GetRelativeSize();
	quitText->SetRelativePosition(CoordHelper::RatioCoordToScreen(quitTextRectSize, FVector2(-0.5, -0.5)));
	quitText->SetRelativeScale(FVector2(1, 1));
	quitText->SetRelativeRotation(0);
	quitText->m_layer = 501;
	quitText->RemoveFromParent();
	quitButton->AddChildComponent(quitText);

	// ======================== closeText
	closeText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	closeText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	closeText->SetText(L"닫기");
	closeText->SetFontSize(55.0f);
	closeText->SetColor(FColor::White);
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
		tutorial, startButton, quitButton, staffButton, optionButton, closeButton, closeText,
		uiSound
	]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());

		if (uiSound->IsPlaying())
			uiSound->Stop();
		
		uiSound->Play(0.45);

		// 다른 버튼 비활성화
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

	continueButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText, PopupTab,
		uiSound, continueTabText
	]()		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());

			if (uiSound->IsPlaying())
				uiSound->Stop();

			uiSound->Play(0.45);

			// 다른 버튼 비활성화
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);

			closeButton->m_layer = 503;
			closeButton->SetActive(true);
			closeText->m_layer = 504;

			continueTabText->m_layer = 503;

			PopupTab->m_layer = 502;
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [
		tutorial, startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText , PopupTab,
		uiSound, continueTabText, optionTabText, staffTabText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");

			if (uiSound->IsPlaying())
				uiSound->Stop();

			uiSound->Play(0.45);

			// 모두 활성화
			startButton->SetActive(true);
			continueButton->SetActive(true);
			quitButton->SetActive(true);
			staffButton->SetActive(true);
			optionButton->SetActive(true);

			closeButton->m_layer = -1000;
			closeButton->SetActive(false);
			closeText->m_layer = -1000;

			continueTabText->m_layer = -1000;
			optionTabText->m_layer = -1000;
			staffTabText->m_layer = -1000;

			tutorial->m_layer = -1000;
			tutorial->Stop();

			PopupTab->m_layer = -1000;
		});

	optionButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab, closeButton, closeText,
		uiSound, optionTabText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			
			if (uiSound->IsPlaying())
				uiSound->Stop();

			uiSound->Play(0.45);

			// 다른 버튼 비활성화
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);
			
			closeButton->m_layer = 503;
			closeButton->SetActive(true);
			closeText->m_layer = 504;

			optionTabText->m_layer = 503;

			PopupTab->m_layer = 502;
		});

	staffButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab, closeButton, closeText,
		uiSound, staffTabText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			if (uiSound->IsPlaying())
				uiSound->Stop();

			uiSound->Play(0.45);

			// 다른 버튼 비활성화
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);

			closeButton->m_layer = 503;
			closeButton->SetActive(true);
			closeText->m_layer = 504;

			staffTabText->m_layer = 503;

			PopupTab->m_layer = 502;
		});

	quitButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			//uiSound->Play(0.45);
			// Quit
			//PostQuitMessage(0);
			// 임시 씬 전환
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
