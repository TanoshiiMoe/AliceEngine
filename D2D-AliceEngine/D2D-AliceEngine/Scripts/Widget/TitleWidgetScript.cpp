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
#include <Component/ProgressBarComponent.h>

#include <Application.h>

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
	auto optionTabBGMText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabSFXText = m_owner->AddComponent<TextRenderComponent>();

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

	auto bgmControl = m_owner->AddComponent<ProgressBarComponent>();
	auto sfxControl = m_owner->AddComponent<ProgressBarComponent>();

	auto bgmPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto bgmMinusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxMinusButton = m_owner->AddComponent<ButtonComponent>();

	auto skipText = m_owner->AddComponent<TextRenderComponent>();
	auto skipButton = m_owner->AddComponent<ButtonComponent>();

	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	auto sound = bgmObj->GetComponent<AudioComponent>();
	
	float bgmVolume = AudioManager::GetInstance().GetBGMVolume();
	float sfxVolume = AudioManager::GetInstance().GetSFXVolume();

    auto soundControl = m_owner->AddComponent<SpriteRenderer>();
    soundControl->LoadData(L"UI\\UI_SoundController.png");
    soundControl->m_layer = Define::Disable;
    soundControl->SetRelativeScale(soundUISize);
    soundControl->SetDrawType(EDrawType::ScreenSpace);
    // 중앙 배치
    soundControl->SetRelativePosition(FVector2(0.f, 0.f));

	bgmControl->SetDrawType(Define::EDrawType::ScreenSpace);
	bgmControl->LoadData(L"UI\\ControlBar.png");
	bgmControl->SetType(EProgressBarType::Linear);
	bgmControl->SetRelativeScale(FVector2(1, 1));
	bgmControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(bgmControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(5, 3));
	bgmControl->SetProgress(bgmVolume);
	bgmControl->m_layer = Define::Disable;

	sfxControl->SetDrawType(Define::EDrawType::ScreenSpace);
	sfxControl->LoadData(L"UI\\ControlBar.png");
	sfxControl->SetType(EProgressBarType::Linear);
	sfxControl->SetRelativeScale(FVector2(1, 1));
	sfxControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(sfxControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(5, 66));
	sfxControl->SetProgress(sfxVolume);
	sfxControl->m_layer = Define::Disable;

	//auto uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	//uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);	// UISound is Included in SFX.

	auto tutorial = m_owner->AddComponent<VideoComponent>();
	tutorial->LoadData(L"BackGround\\Mari_Sportswear.webm",30,L"jpg",95,true);
	tutorial->m_layer = Define::Disable;
	tutorial->SetRelativePosition(FVector2(-960, -550));

	auto PopupTab = m_owner->AddComponent<SpriteRenderer>();
	PopupTab->LoadData(L"UI\\PopupTab.png");
	PopupTab->m_layer = Define::Disable;
	PopupTab->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f , -SCREEN_HEIGHT / 2.0f));

	if (!startText || !startButton) return;
	if (!continueText || !continueButton || !continueTabText) return;
	if (!optionText || !optionButton || !optionTabText) return;
	if (!staffText || !staffButton || !staffTabText) return;
	if (!quitText || !quitButton) return;
	if (!tutorial || !closeText || !closeButton) return;
	if (!sound || !PopupTab) return;

	float buttonBasePos = 400;

	// ======================== startButton
	//startButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	//startButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	//FVector2 startButtonSize = startButton->GetRelativeSize();
	//startButton->SetRelativePosition(CoordHelper::RatioCoordToScreen(startButtonSize, FVector2(1, 0))
	//	+ FVector2(buttonBasePos, -150));
	//startButton->SetRelativeScale(FVector2(1, 1));
	//startButton->m_layer = Define::ButtonLayer;

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
	continueButton->m_layer = Define::ButtonLayer;

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
	optionButton->m_layer = Define::ButtonLayer;

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
	staffButton->m_layer = Define::ButtonLayer;

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
	quitButton->m_layer = Define::ButtonLayer;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	closeButton->SetRelativePosition(FVector2(0,350));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetActive(false);
	closeButton->m_layer = Define::Disable;

	smallClose->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
    smallClose->SetDrawType(EDrawType::ScreenSpace);
    smallClose->SetRelativeScale(0.43f);
    smallClose->SetActive(false);
    smallClose->m_layer = Define::Disable;
    // 패널 우상단 모서리 근처에 배치
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float hh = soundControl->GetBitmapSizeY() * soundUISize * 0.5f;
        const float margin = 20.f;
        smallClose->SetRelativePosition(FVector2(hw - margin, -hh + margin));
    }

	// ======================== skipButton
	skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	skipButton->SetRelativePosition(FVector2(0, 350));
	skipButton->SetRelativeScale(FVector2(1, 1));
	skipButton->m_layer = Define::Disable;

	// ======================== soundButton
	bgmPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	bgmPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 100.f;
		const float marginY = 82.5f;
        const float rowY = -panelH * 0.25f; // 위쪽 라인
        bgmPlusButton->SetRelativePosition(FVector2(hw - marginX, rowY + marginY));
    }
	bgmPlusButton->SetActive(false);
	bgmPlusButton->m_layer = Define::Disable;

	bgmMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	bgmMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	bgmMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 110.f;
		const float marginY = 82.5f;
        const float rowY = -panelH * 0.25f;
        bgmMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY + marginY));
    }
	bgmMinusButton->SetActive(false);
	bgmMinusButton->m_layer = Define::Disable;

	sfxPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	sfxPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	sfxPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 100.f;
		const float marginY = 11.0f;
        const float rowY = panelH * 0.25f; // 아래쪽 라인
        sfxPlusButton->SetRelativePosition(FVector2(hw - marginX, rowY - marginY));
    }
	sfxPlusButton->SetActive(false);
	sfxPlusButton->m_layer = Define::Disable;

	sfxMinusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundMinus_Idle.png");
	sfxMinusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundMinus_Pressed.png");
	sfxMinusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundMinus_Idle.png");
    {
        const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
        const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = 110.5f;
		const float marginY = 10.7f;
        const float rowY = panelH * 0.25f;
        sfxMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY - marginY));
    }
	sfxMinusButton->SetActive(false);
	sfxMinusButton->m_layer = Define::Disable;

	// ======================== mainTitle
	mainTitle->SetFontSize(120.0f);
	mainTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	mainTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	mainTitle->SetText(L"높다락길의 질주");
	mainTitle->SetColor(FColor(242, 207, 238, 255));
	FVector2 mainTitleRectSize = mainTitle->GetRelativeSize();
	mainTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, -300));
	mainTitle->SetRelativeScale(FVector2(1, 1));
	mainTitle->SetRelativeRotation(0);
	mainTitle->m_layer = Define::NormalTextLayer;

	// ======================== subTitle
	subTitle->SetFontSize(60.0f);
	subTitle->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	subTitle->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	subTitle->SetText(L"~활빈은 두령을 쫒는다~");
	subTitle->SetColor(FColor(242, 207, 238, 255));
	FVector2 subTitleRectSize = subTitle->GetRelativeSize();
	subTitle->SetRelativePosition(CoordHelper::RatioCoordToScreen(subTitleRectSize, FVector2(-0.5, -0.5)) + FVector2(300, 0));
	subTitle->SetRelativeScale(FVector2(1, 1));
	subTitle->SetRelativeRotation(0);
	subTitle->m_layer = Define::NormalTextLayer;

	// ======================== startText
	//startText->SetFontSize(55.0f);
	//startText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//startText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//startText->SetText(L"시작하기");
	//startText->SetColor(FColor::White);
	//FVector2 startTextRectSize = startText->GetRelativeSize();
	//startText->SetRelativePosition(CoordHelper::RatioCoordToScreen(startTextRectSize, FVector2(-0.5, -0.5)));
	//startText->SetRelativeScale(FVector2(1, 1));
	//startText->SetRelativeRotation(0);
	//startText->m_layer = Define::ButtonTextLayer;
	//startText->RemoveFromParent();
	//startButton->AddChildComponent(startText);

	// ======================== continueText
	continueText->SetFontSize(55.0f);
	continueText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	continueText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	continueText->SetText(L"임무 선택");
	continueText->SetColor(FColor::White);
	FVector2 continueTextRectSize = continueText->GetRelativeSize();
	continueText->SetRelativePosition(CoordHelper::RatioCoordToScreen(continueTextRectSize, FVector2(-0.5, -0.5)));
	continueText->SetRelativeScale(FVector2(1, 1));
	continueText->SetRelativeRotation(0);
	continueText->m_layer = Define::ButtonTextLayer;
	continueText->RemoveFromParent();
	continueButton->AddChildComponent(continueText);

	//continueTabText->SetFontSize(70.0f);
	//continueTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	//continueTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	//continueTabText->SetText(L"임무 선택");
	//continueTabText->SetColor(FColor(0, 234, 255, 255));
	//FVector2 continueTabTextRectSize = continueTabText->GetRelativeSize();
	//continueTabText->SetRelativePosition(
	//	CoordHelper::RatioCoordToScreen(continueTabTextRectSize, FVector2(-0.5, -0.5))
	//	+ FVector2(0, -350)
	//);
	//continueTabText->SetRelativeRotation(0);
	//continueTabText->m_layer = -1000;

	// ======================== optionText
	optionText->SetFontSize(55.0f);
	optionText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionText->SetText(L"음량 조절");
	optionText->SetColor(FColor::White);
	FVector2 optionTextRectSize = optionText->GetRelativeSize();
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionTextRectSize, FVector2(-0.5, -0.5)));
	optionText->SetRelativeScale(FVector2(1, 1));
	optionText->SetRelativeRotation(0);
	optionText->m_layer = Define::ButtonTextLayer;
	optionText->RemoveFromParent();
	optionButton->AddChildComponent(optionText);

	optionTabText->SetFontSize(55.0f);
	optionTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabText->SetText(L"음량 조절");
	optionTabText->SetColor(FColor::White);
	FVector2 optionTabTextRectSize = optionTabText->GetRelativeSize();
	optionTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -85)
	);
	optionTabText->SetRelativeRotation(0);
	optionTabText->m_layer = Define::Disable;

	optionTabBGMText->SetFontSize(20.0f);
	optionTabBGMText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabBGMText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabBGMText->SetText(L"배경음");
	optionTabBGMText->SetColor(FColor(0, 234, 255, 255));
	optionTabBGMText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabBGMText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, -30)
	);
	optionTabBGMText->SetRelativeRotation(0);
	optionTabBGMText->m_layer = Define::Disable;

	optionTabSFXText->SetFontSize(20.0f);
	optionTabSFXText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabSFXText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabSFXText->SetText(L"효과음");
	optionTabSFXText->SetColor(FColor(0, 234, 255, 255));
	optionTabSFXText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabSFXText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 35)
	);
	optionTabSFXText->SetRelativeRotation(0);
	optionTabSFXText->m_layer = Define::Disable;

	// ======================== staffText
	staffText->SetFontSize(55.0f);
	staffText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffText->SetText(L"제작진");
	staffText->SetColor(FColor::White);
	FVector2 staffTextRectSize = continueText->GetRelativeSize();
	staffText->SetRelativePosition(CoordHelper::RatioCoordToScreen(staffTextRectSize, FVector2(-0.5, -0.5)) + FVector2(20,0));
	staffText->SetRelativeScale(FVector2(1, 1));
	staffText->SetRelativeRotation(0);
	staffText->m_layer = Define::ButtonTextLayer;
	staffText->RemoveFromParent();
	staffButton->AddChildComponent(staffText);

	staffTabText->SetFontSize(70.0f);
	staffTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	staffTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	staffTabText->SetText(L"제작진");
	staffTabText->SetColor(FColor(0, 234, 255, 255));
	FVector2 staffTabTextSize = staffTabText->GetRelativeSize();
	staffTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffTabTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, -350)
	);
	staffTabText->SetRelativeRotation(0);
	staffTabText->m_layer = Define::Disable;

	staffNameText->SetFontSize(55.0f);
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
	staffNameText->SetColor(FColor::Black);
	FVector2 staffNameTextSize = staffNameText->GetRelativeSize();
	staffNameText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(staffNameTextSize, FVector2(-0.5, -0.5))
		+ FVector2(0, 50)
	);
	staffNameText->m_layer = Define::Disable;

	// ======================== quitText
	quitText->SetFontSize(55.0f);
	quitText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	quitText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	quitText->SetText(L"오락 종료");
	quitText->SetColor(FColor::White);
	FVector2 quitTextRectSize = quitText->GetRelativeSize();
	quitText->SetRelativePosition(CoordHelper::RatioCoordToScreen(quitTextRectSize, FVector2(-0.5, -0.5)));
	quitText->SetRelativeScale(FVector2(1, 1));
	quitText->SetRelativeRotation(0);
	quitText->m_layer = Define::ButtonTextLayer;
	quitText->RemoveFromParent();
	quitButton->AddChildComponent(quitText);

	// ======================== closeText
	closeText->SetFontSize(55.0f);
	closeText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	closeText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	closeText->SetText(L"닫기");
	closeText->SetColor(FColor::White);
	FVector2 closeTextRectSize = closeText->GetRelativeSize();
	closeText->SetRelativePosition(CoordHelper::RatioCoordToScreen(closeTextRectSize, FVector2(-0.5, -0.5)));
	closeText->SetRelativeScale(FVector2(1, 1));
	closeText->SetRelativeRotation(0);
	closeText->m_layer = Define::Disable;
	closeButton->SetActive(false);
	closeText->RemoveFromParent();
	closeButton->AddChildComponent(closeText);

	// ======================== skipText
	skipText->SetFontSize(55.0f);
	skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	skipText->SetText(L"건너뛰기");
	skipText->SetColor(FColor::White);
	FVector2 skipTextRectSize = skipText->GetRelativeSize();
	skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextRectSize, FVector2(-0.5, -0.5)));
	skipText->SetRelativeScale(FVector2(1, 1));
	skipText->SetRelativeRotation(0);
	skipText->m_layer = Define::Disable;
	skipButton->SetActive(false);
	skipText->RemoveFromParent();
	skipButton->AddChildComponent(skipText);

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
		sound, continueButton, skipButton, skipText
	]()
	{
		OutputDebugStringW(L"SetAction click!\n");
		OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		//uiSound->StopByName(L"UISound");
		//uiSound->PlayByName1(L"UISound", 0.45f);

		// 다른 버튼 비활성화 (애니메이션 자동 중지됨)
		startButton->SetActive(false);
		quitButton->SetActive(false);
		staffButton->SetActive(false);
		optionButton->SetActive(false);
		continueButton->SetActive(false);

		//closeButton->m_layer = 503;
		//closeButton->SetActive(true);
		//closeText->m_layer = 504;

		skipButton->m_layer = Define::PopupButtonLayer;
		skipButton->SetActive(true);
		skipText->m_layer = Define::PopupTextLayer;

		tutorial->m_layer = Define::PopupLayer;
		tutorial->Play();
	});

	skipButton->SetStateAction(Define::EButtonState::Pressed, []()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			SceneManager::ChangeScene(Define::Scene_Stage1);
		});


	continueButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, closeButton, closeText, PopupTab,
		sound, continueTabText
	]()		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			//uiSound->StopByName(L"UISound");
			//uiSound->PlayByName1(L"UISound", 0.45f);
			
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
		sound, continueTabText, optionTabText, staffTabText, staffNameText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			//uiSound->StopByName(L"UISound");
			//uiSound->PlayByName1(L"UISound", 0.45f);

			// 모두 활성화
			startButton->SetActive(true);
			continueButton->SetActive(true);
			quitButton->SetActive(true);
			staffButton->SetActive(true);
			optionButton->SetActive(true);

			closeButton->m_layer = Define::Disable;
			closeButton->SetActive(false);
			closeText->m_layer = Define::Disable;

			continueTabText->m_layer = Define::Disable;
			//optionTabText->m_layer = -1000;
			staffTabText->m_layer = Define::Disable;
			staffNameText->m_layer = Define::Disable;

			tutorial->m_layer = Define::Disable;
			tutorial->Stop();

			PopupTab->m_layer = Define::Disable;
		});

	smallClose->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, smallClose,
		bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl, sound, optionTabText,
		optionTabBGMText, optionTabSFXText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		startButton->SetActive(true);
		continueButton->SetActive(true);
		quitButton->SetActive(true);
		staffButton->SetActive(true);
		optionButton->SetActive(true);

		smallClose->m_layer = Define::Disable;
		smallClose->SetActive(false);

		bgmMinusButton->SetActive(false);
		bgmPlusButton->SetActive(false);
		bgmMinusButton->m_layer = Define::Disable;
		bgmPlusButton->m_layer = Define::Disable;

		sfxPlusButton->SetActive(false);
		sfxMinusButton->SetActive(false);
		sfxPlusButton->m_layer = Define::Disable;
		sfxMinusButton->m_layer = Define::Disable;

		optionTabText->m_layer = Define::Disable;
		optionTabBGMText->m_layer = Define::Disable;
		optionTabSFXText->m_layer = Define::Disable;

		sfxControl->m_layer = Define::Disable;
		bgmControl->m_layer = Define::Disable;
		soundControl->m_layer = Define::Disable;
		});

	optionButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab,
		sound, optionTabText, bgmMinusButton, bgmPlusButton, sfxPlusButton, sfxMinusButton,
		sfxControl, bgmControl, soundControl, smallClose
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			//SceneManager::ChangeScene(L"HiroScene");
			
			//if (uiSound->IsPlaying())
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			// 다른 버튼 비활성화 (애니메이션 자동 중지됨)
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);
			
			smallClose->SetActive(true);
			smallClose->m_layer = Define::PopupButtonLayer;

			//optionTabText->m_layer = 503;
			bgmMinusButton->SetActive(true);
			bgmPlusButton->SetActive(true);
			bgmMinusButton->m_layer = Define::PopupButtonLayer;
			bgmPlusButton->m_layer = Define::PopupButtonLayer;

			sfxPlusButton->SetActive(true);
			sfxMinusButton->SetActive(true);
			sfxPlusButton->m_layer = Define::PopupButtonLayer;
			sfxMinusButton->m_layer = Define::PopupButtonLayer;

			optionTabText->m_layer = Define::PopupTextLayer;

			sfxControl->m_layer = Define::PopupObjectLayer;
			bgmControl->m_layer = Define::PopupObjectLayer;
			soundControl->m_layer = Define::PopupLayer;
		});

	staffButton->SetStateAction(Define::EButtonState::Pressed, [
		startButton, continueButton, quitButton, staffButton, optionButton, PopupTab, closeButton, closeText,
		sound, staffTabText, staffNameText
	]()
		{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());
			
			//if (uiSound->IsPlaying())
			//uiSound->StopByName(L"UISound");

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			// 다른 버튼 비활성화 (애니메이션 자동 중지됨)
			startButton->SetActive(false);
			continueButton->SetActive(false);
			quitButton->SetActive(false);
			staffButton->SetActive(false);
			optionButton->SetActive(false);

			closeButton->m_layer = Define::PopupButtonLayer;
			closeButton->SetActive(true);
			closeText->m_layer = Define::PopupTextLayer;

			staffTabText->m_layer = Define::PopupTextLayer;
			staffNameText->m_layer = Define::PopupTextLayer;

			PopupTab->m_layer = Define::PopupLayer;
		});

	// bgmVolume
    bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl]
		{
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetBGMVolume();
			vol -= 0.1f;
			AudioManager::GetInstance().SetBGMVolume(vol);

			bgmControl->SetProgress(vol);
		});

    bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl]
		{
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetBGMVolume();
			vol += 0.1f;
			AudioManager::GetInstance().SetBGMVolume(vol);

			bgmControl->SetProgress(vol);
		});

    sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [
        sound, sfxControl
    ] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			float vol = AudioManager::GetInstance().GetSFXVolume();
			vol -= 0.1f;
			AudioManager::GetInstance().SetSFXVolume(vol);

			sfxControl->SetProgress(vol);
		});


    sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [
        sound, sfxControl
    ] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");
		
			float vol = AudioManager::GetInstance().GetSFXVolume();
			vol += 0.1f;
			AudioManager::GetInstance().SetSFXVolume(vol);

			sfxControl->SetProgress(vol);
		});


	quitButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
			OutputDebugStringW(L"SetAction click!\n");
			OutputDebugStringW((L"x,y " + std::to_wstring(Input::GetMousePosition().x) + L", " + std::to_wstring(Input::GetMousePosition().y) + L"\n").c_str());

			// Quit
			//PostQuitMessage(0);
			Application::GetInstance().GameQuit();
		});

	quitButton->SetStateAction(Define::EButtonState::Hover, [quitButton]()
	{
		quitButton->StartHoverPulse(0.8f, 0.04f);
		quitButton->StartEffectAnimation(0.3f, 1.2f, FColor::White);
	});

	quitButton->SetStateAction(Define::EButtonState::HoverLeave, [quitButton]()
	{
		quitButton->StopHoverPulse();
		quitButton->StartEffectAnimation(0.2f, 0.0f, FColor::White);
	});

	quitButton->SetStateAction(Define::EButtonState::Release, [quitButton]()
	{
		quitButton->StopHoverPulse();
		quitButton->StartEffectAnimation(0.1f, 0.0f, FColor::White);
	});

	// optionButton Hover 효과 (노란색 글로우)
	optionButton->SetStateAction(Define::EButtonState::Hover, [optionButton]()
	{
		optionButton->StartHoverPulse(0.8f, 0.04f);
		optionButton->StartEffectAnimation(0.3f, 1.2f, FColor::Yellow);
	});

	optionButton->SetStateAction(Define::EButtonState::HoverLeave, [optionButton]()
	{
		optionButton->StopHoverPulse();
		optionButton->StartEffectAnimation(0.2f, 0.0f, FColor::Yellow);
	});

	optionButton->SetStateAction(Define::EButtonState::Release, [optionButton]()
	{
		optionButton->StopHoverPulse();
		optionButton->StartEffectAnimation(0.1f, 0.0f, FColor::Yellow);
	});

	// staffButton Hover 효과 (보라색 글로우)
	staffButton->SetStateAction(Define::EButtonState::Hover, [staffButton]()
	{
		staffButton->StartHoverPulse(0.8f, 0.04f);
		staffButton->StartEffectAnimation(0.3f, 1.2f, FColor::Purple);
	});

	staffButton->SetStateAction(Define::EButtonState::HoverLeave, [staffButton]()
	{
		staffButton->StopHoverPulse();
		staffButton->StartEffectAnimation(0.2f, 0.0f, FColor::Purple);
	});

	staffButton->SetStateAction(Define::EButtonState::Release, [staffButton]()
	{
		staffButton->StopHoverPulse();
		staffButton->StartEffectAnimation(0.1f, 0.0f, FColor::Purple);
	});

	// continueButton Hover 효과 (초록색 글로우)
	continueButton->SetStateAction(Define::EButtonState::Hover, [continueButton]()
	{
		continueButton->StartHoverPulse(0.8f, 0.04f);
		continueButton->StartEffectAnimation(0.3f, 1.2f, FColor::Green);
	});

	continueButton->SetStateAction(Define::EButtonState::HoverLeave, [continueButton]()
	{
		continueButton->StopHoverPulse();
		continueButton->StartEffectAnimation(0.2f, 0.0f, FColor::Green);
	});

	continueButton->SetStateAction(Define::EButtonState::Release, [continueButton]()
	{
		continueButton->StopHoverPulse();
		continueButton->StartEffectAnimation(0.1f, 0.0f, FColor::Green);
	});

	// closeButton Hover 효과 (주황색 글로우)
	closeButton->SetStateAction(Define::EButtonState::Hover, [closeButton]()
	{
		closeButton->StartHoverPulse(0.8f, 0.04f);
		closeButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	});

	closeButton->SetStateAction(Define::EButtonState::HoverLeave, [closeButton]()
	{
		closeButton->StopHoverPulse();
		closeButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	});

	closeButton->SetStateAction(Define::EButtonState::Release, [closeButton]()
	{
		closeButton->StopHoverPulse();
		closeButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	});

	// bgmPlusButton Hover 효과 (시안색 글로우)
	bgmPlusButton->SetStateAction(Define::EButtonState::Hover, [bgmPlusButton]()
	{
		bgmPlusButton->StartHoverPulse(0.8f, 0.04f);
		bgmPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Cyan);
	});

	bgmPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmPlusButton]()
	{
		bgmPlusButton->StopHoverPulse();
		bgmPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Cyan);
	});

	bgmPlusButton->SetStateAction(Define::EButtonState::Release, [bgmPlusButton]()
	{
		bgmPlusButton->StopHoverPulse();
		bgmPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Cyan);
	});

	// bgmMinusButton Hover 효과 (마젠타색 글로우)
	bgmMinusButton->SetStateAction(Define::EButtonState::Hover, [bgmMinusButton]()
	{
		bgmMinusButton->StartHoverPulse(0.8f, 0.04f);
		bgmMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Magenta);
	});

	bgmMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmMinusButton]()
	{
		bgmMinusButton->StopHoverPulse();
		bgmMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Magenta);
	});

	bgmMinusButton->SetStateAction(Define::EButtonState::Release, [bgmMinusButton]()
	{
		bgmMinusButton->StopHoverPulse();
		bgmMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Magenta);
	});

	// sfxPlusButton Hover 효과 (금색 글로우)
	sfxPlusButton->SetStateAction(Define::EButtonState::Hover, [sfxPlusButton]()
	{
		sfxPlusButton->StartHoverPulse(0.8f, 0.04f);
		sfxPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Gold);
	});

	sfxPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxPlusButton]()
	{
		sfxPlusButton->StopHoverPulse();
		sfxPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Gold);
	});

	sfxPlusButton->SetStateAction(Define::EButtonState::Release, [sfxPlusButton]()
	{
		sfxPlusButton->StopHoverPulse();
		sfxPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Gold);
	});

	// sfxMinusButton Hover 효과 (분홍색 글로우)
	sfxMinusButton->SetStateAction(Define::EButtonState::Hover, [sfxMinusButton]()
	{
		sfxMinusButton->StartHoverPulse(0.8f, 0.04f);
		sfxMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor::Pink);
	});

	sfxMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxMinusButton]()
	{
		sfxMinusButton->StopHoverPulse();
		sfxMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor::Pink);
	});

	sfxMinusButton->SetStateAction(Define::EButtonState::Release, [sfxMinusButton]()
	{
		sfxMinusButton->StopHoverPulse();
		sfxMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor::Pink);
	});

	// startButton Hover 효과 (펄스 + 글로우 이펙트)
	startButton->SetStateAction(Define::EButtonState::Hover, [startButton]()
	{
		startButton->StartHoverPulse(0.8f, 0.04f);
		// 빨간색 글로우 - 훨씬 간단해짐!
		startButton->StartEffectAnimation(0.3f, 1.2f, FColor::Red);
	});

	startButton->SetStateAction(Define::EButtonState::HoverLeave, [startButton]()
	{
		startButton->StopHoverPulse();
		startButton->StartEffectAnimation(0.2f, 0.0f, FColor::Red);
	});

	startButton->SetStateAction(Define::EButtonState::Release, [startButton]()
	{
		startButton->StopHoverPulse();
		startButton->StartEffectAnimation(0.1f, 0.0f, FColor::Red); 
	});
}

void TitleWidgetScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	
}

void TitleWidgetScript::OnDestroy()
{

}
