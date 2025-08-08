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
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>

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

	float guargeSize = 1.0f;
	float soundUISize = 1.0f;

	auto background = m_owner->AddComponent<SpriteRenderer>();

	auto startText = m_owner->AddComponent<TextRenderComponent>();
	auto startButton = m_owner->AddComponent<ButtonComponent>();
	//startButton->RemoveFromParent();
	//GetCamera()->AddChildObject(startButton);

	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto continueButton = m_owner->AddComponent<ButtonComponent>();
	auto continueTabText = m_owner->AddComponent<TextRenderComponent>();

	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto optionButton = m_owner->AddComponent<ButtonComponent>();
	auto optionTabText = m_owner->AddComponent<TextRenderComponent>();

	auto staffText = m_owner->AddComponent<TextRenderComponent>();
	auto staffButton = m_owner->AddComponent<ButtonComponent>();
	auto staffTabText = m_owner->AddComponent<TextRenderComponent>();
	auto staffNameText = m_owner->AddComponent<TextRenderComponent>();

	auto quitText = m_owner->AddComponent<TextRenderComponent>();
	auto quitButton = m_owner->AddComponent<ButtonComponent>();

	auto closeText = m_owner->AddComponent<TextRenderComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();
	auto smallClose = m_owner->AddComponent<ButtonComponent>();

	auto mainTitle = m_owner->AddComponent<TextRenderComponent>();
	auto subTitle = m_owner->AddComponent<TextRenderComponent>();

	auto bgmPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto bgmMinusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxMinusButton = m_owner->AddComponent<ButtonComponent>();

	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	auto bgm = bgmObj->GetComponent<AudioComponent>();
	
	float bgmVolume = AudioManager::GetInstance().GetBGMVolume();
	float sfxVolume = AudioManager::GetInstance().GetSFXVolume();

	auto soundControl = m_owner->AddComponent<SpriteRenderer>();
	soundControl->LoadData(L"UI\\UI_SoundControl.png");
	soundControl->m_layer = -1000;
	soundControl->SetRelativeScale(soundUISize);
	soundControl->SetDrawType(EDrawType::ScreenSpace);
	soundControl->SetRelativePosition(FVector2(300, 45));

	auto bgmControl = m_owner->AddComponent<SpriteRenderer>();
	bgmControl->LoadData(L"UI\\BGMControl.png");
	bgmControl->m_layer = -1000;
	bgmControl->SetDrawType(EDrawType::ScreenSpace);
	bgmControl->SetRelativeScale(FVector2(bgmVolume * soundUISize, soundUISize));

	auto sfxControl = m_owner->AddComponent<SpriteRenderer>();
	sfxControl->LoadData(L"UI\\SFXControl.png");
	sfxControl->m_layer = -1000;
	sfxControl->SetDrawType(EDrawType::ScreenSpace);
	/*float finalPos = -SCREEN_WIDTH / 2.0f
		+ (sfxControl->GetBitmapSizeX() * soundUISize / 2.0f) * (bgm->GetVolume(SoundType::BGM) - 1);
	sfxControl->SetRelativePosition(FVector2(finalPos, -SCREEN_HEIGHT / 2.0f - 10));*/
	//sfxControl->SetRelativeScale(FVector2(bgm->GetVolume(SoundType::SFX) * soundUISize, soundUISize));
	
	sfxControl->SetSlice(0, 0, sfxControl->GetBitmapSizeX() * sfxVolume, sfxControl->GetBitmapSizeY());

	auto uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);	// UISound is Included in SFX.

	auto tutorial = m_owner->AddComponent<VideoComponent>();
	tutorial->LoadData(L"BackGround\\Mari_Sportswear.webm",30,L"jpg",95,true);
	tutorial->m_layer = -1000;
	tutorial->SetRelativePosition(FVector2(-960, -550));

	auto PopupTab = m_owner->AddComponent<SpriteRenderer>();
	PopupTab->LoadData(L"UI\\PopupTab.png");
	PopupTab->m_layer = -1000;
	PopupTab->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f , -SCREEN_HEIGHT / 2.0f));

	if (!startText || !startButton) return;
	if (!continueText || !continueButton || !continueTabText) return;
	if (!optionText || !optionButton || !optionTabText) return;
	if (!staffText || !staffButton || !staffTabText) return;
	if (!quitText || !quitButton) return;
	if (!tutorial || !closeText || !closeButton) return;
	if (!uiSound || !PopupTab) return;

	float buttonBasePos = 400;

	// ======================== startButton
	startButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	startButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 startButtonSize = startButton->GetRelativeSize();
	startButton->SetRelativePosition(CoordHelper::RatioCoordToScreen(startButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, -150));
	startButton->SetRelativeScale(FVector2(1, 1));
	startButton->m_layer = 300;

	// ======================== continueButton
	continueButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	continueButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 continueButtonSize = continueButton->GetRelativeSize();
	continueButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 0));
	continueButton->SetRelativeScale(FVector2(1, 1));
	continueButton->m_layer = 500;

	// ======================== optionButton
	optionButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	optionButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 optionButtonSize = optionButton->GetRelativeSize();
	optionButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 150));
	optionButton->SetRelativeScale(FVector2(1, 1));
	optionButton->m_layer = 500;

	// ======================== staffButton
	staffButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	staffButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	FVector2 staffButtonSize = staffButton->GetRelativeSize();
	staffButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffButtonSize, FVector2(1, 0))
		+ FVector2(buttonBasePos, 300));
	staffButton->SetRelativeScale(FVector2(1, 1));
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
	closeButton->SetActive(false);
	closeButton->m_layer = -1000;

	smallClose->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	smallClose->SetDrawType(EDrawType::ScreenSpace);
	smallClose->SetRelativeScale(0.43f);
	smallClose->SetActive(false);
	smallClose->m_layer = -1000;
	smallClose->SetRelativePosition(FVector2(279 + soundControl->GetBitmapSizeX() / 2.0f, 68 - soundControl->GetBitmapSizeY()/ 2.0f));

	// ======================== soundButton
	bgmPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	bgmPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->SetRelativePosition(FVector2(445, 50.7));
	bgmPlusButton->SetActive(false);
	bgmPlusButton->m_layer = -1000;

	bgmMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	bgmMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->SetRelativePosition(FVector2(164, 50.7));
	bgmMinusButton->SetActive(false);
	bgmMinusButton->m_layer = -1000;

	sfxPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	sfxPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->SetRelativePosition(FVector2(445, 114));
	sfxPlusButton->SetActive(false);
	sfxPlusButton->m_layer = -1000;

	sfxMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	sfxMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->SetRelativePosition(FVector2(164, 114));
	sfxMinusButton->SetActive(false);
	sfxMinusButton->m_layer = -1000;

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

	staffNameText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffNameText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffNameText->SetText(
		L"기획\n"
		L"이유성 | 신소영\n"
		L"\n"
		L"아트\n"
		L"강연주 | 민지인\n"
		L"\n"
		L"프로그래밍\n"
		L"이창진 | 강성근 | 황태현\n"
	);
	staffNameText->SetFontSize(55.0f);
	staffNameText->SetColor(FColor::Black);
	FVector2 staffNameTextSize = staffNameText->GetRelativeSize();
	staffNameText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffNameTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, 50)
	);
	staffNameText->m_layer = -1000;

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
		uiSound, continueButton
	]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());

		if (uiSound->IsPlaying())
			uiSound->StopByName(L"UISound");
		
		uiSound->PlayByName(L"UISound",0.45f);

		// 다른 버튼 비활성화
		startButton->SetActive(false);
		quitButton->SetActive(false);
		staffButton->SetActive(false);
		optionButton->SetActive(false);
		continueButton->SetActive(false);

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
				uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);
			
			// 여기에 몇 초 뒤 씬 전환 넣으면 될 거 같음
			SceneManager::ChangeScene(L"SelectScene");

			// 다른 버튼 비활성화
			//startButton->SetActive(false);
			//continueButton->SetActive(false);
			//quitButton->SetActive(false);
			//staffButton->SetActive(false);
			//optionButton->SetActive(false);

			//closeButton->m_layer = 503;
			//closeButton->SetActive(true);
			//closeText->m_layer = 504;

			//continueTabText->m_layer = 503;

			//PopupTab->m_layer = 502;
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [
		tutorial, startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText , PopupTab,
		uiSound, continueTabText, optionTabText, staffTabText, staffNameText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");

			if (uiSound->IsPlaying())
				uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);

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
			//optionTabText->m_layer = -1000;
			staffTabText->m_layer = -1000;
			staffNameText->m_layer = -1000;

			tutorial->m_layer = -1000;
			tutorial->Stop();

			PopupTab->m_layer = -1000;
		});

	smallClose->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, smallClose,
		bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl
	] {
		startButton->SetActive(true);
		continueButton->SetActive(true);
		quitButton->SetActive(true);
		staffButton->SetActive(true);
		optionButton->SetActive(true);

		smallClose->m_layer = -1000;
		smallClose->SetActive(false);

		bgmMinusButton->SetActive(false);
		bgmPlusButton->SetActive(false);
		bgmMinusButton->m_layer = -1000;
		bgmPlusButton->m_layer = -1000;

		sfxPlusButton->SetActive(false);
		sfxMinusButton->SetActive(false);
		sfxPlusButton->m_layer = -1000;
		sfxMinusButton->m_layer = -1000;

		sfxControl->m_layer = -1000;
		bgmControl->m_layer = -1000;
		soundControl->m_layer = -1000;
		});

	optionButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab,
		uiSound, optionTabText, bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl, smallClose
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			
			if (uiSound->IsPlaying())
				uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);

			// 다른 버튼 비활성화
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);
			
			smallClose->SetActive(true);
			smallClose->m_layer = 504;

			//optionTabText->m_layer = 503;
			bgmMinusButton->SetActive(true);
			bgmPlusButton->SetActive(true);
			bgmMinusButton->m_layer = 503;
			bgmPlusButton->m_layer = 503;

			sfxPlusButton->SetActive(true);
			sfxMinusButton->SetActive(true);
			sfxPlusButton->m_layer = 503;
			sfxMinusButton->m_layer = 503;

			sfxControl->m_layer = 503;
			bgmControl->m_layer = 503;
			soundControl->m_layer = 502;
		});

	staffButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab, closeButton, closeText,
		uiSound, staffTabText, staffNameText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			if (uiSound->IsPlaying())
				uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);

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
			staffNameText->m_layer = 503;

			PopupTab->m_layer = 502;
		});

	// bgmVolume
	bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [bgm, bgmControl, soundUISize, uiSound, &bgmVolume]
		{
			if (uiSound->IsPlaying())
			{
				uiSound->PlayByName(L"UISound", 0.45f);
			}
			else
				uiSound->RestartByName(L"UISound", 0.45f);

			bgm->AddVolumeByType(SoundType::BGM, -0.1);

			float finalPos = -SCREEN_WIDTH / 2.0f
				+ (bgmControl->GetBitmapSizeX() * soundUISize / 2.0f) * (bgmVolume - 1);

			bgmControl->SetRelativePosition(FVector2(finalPos,-SCREEN_HEIGHT / 2.0f -10));
		});

	bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [bgm, sfxControl, soundUISize, uiSound, &bgmVolume]
		{
			if (uiSound->IsPlaying())
			{
				uiSound->PlayByName(L"UISound", 0.45f);
			}
			else
				uiSound->RestartByName(L"UISound", 0.45f);

			bgm->AddVolumeByType(SoundType::BGM, 0.1);

			float finalPos = -SCREEN_WIDTH / 2.0f
				+ (sfxControl->GetBitmapSizeX() * soundUISize / 2.0f) * (bgmVolume - 1);

			sfxControl->SetRelativePosition(FVector2(finalPos, -SCREEN_HEIGHT / 2.0f - 10));
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [
		uiSound, sfxControl, sfxVolume
	] {
			//if (uiSound->IsPlaying())
			//	uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);

			uiSound->AddVolumeByType(SoundType::SFX, -0.1);

			sfxControl->SetSlice(0, 0, sfxControl->GetBitmapSizeX()* AudioManager::GetInstance().GetSFXVolume(), sfxControl->GetBitmapSizeY());
		});


	sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [
		uiSound, sfxControl
	] {
			//if (uiSound->IsPlaying())
			//	uiSound->StopByName(L"UISound");

			uiSound->PlayByName(L"UISound", 0.45f);
		
			uiSound->AddVolumeByType(SoundType::SFX, 0.1);

			float a = AudioManager::GetInstance().GetSFXVolume();
			float b = sfxControl->GetBitmapSizeX();

			sfxControl->SetSlice(0, 0, a * b, sfxControl->GetBitmapSizeY());

		});


	quitButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			//uiSound->Play(0.45);
			// Quit
			//PostQuitMessage(0);
			// 임시 씬 전환
			//SceneManager::ChangeScene(L"HiroScene");
			SceneManager::ChangeScene(L"KangTest");
		});

	quitButton->SetStateAction(Define::EButtonState::Hover, [quitButton]()
	{
		quitButton->StartHoverPulse(0.8f, 0.04f);
	});

	quitButton->SetStateAction(Define::EButtonState::HoverLeave, [quitButton]()
	{
		quitButton->StopHoverPulse();
	});

	quitButton->SetStateAction(Define::EButtonState::Release, [quitButton]()
	{
		quitButton->StopHoverPulse();
	});
	
}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	
}

void TitleWidgetScript::OnDestroy()
{

}
