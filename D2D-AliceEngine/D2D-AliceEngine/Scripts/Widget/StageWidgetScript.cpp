#include "StageWidgetScript.h"

#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Math/Transform.h>
#include <Helpers/CoordHelper.h>

#include <Core/Input.h>
#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/SpriteRenderer.h>
#include <Component/TextRenderComponent.h>
#include <Component/ButtonComponent.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>

#include <GameManager/GamePlayManager.h>
#include <Component/ProgressBarComponent.h>

#include <Scripts/Bike/BikeMovementScript.h>

void StageWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void StageWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	GamePlayManager& gm = GamePlayManager::GetInstance();

	float timeSec = GamePlayManager::GetInstance().GetPassedTime();

	int minutes = static_cast<int>(timeSec) / 60;
	int seconds = static_cast<int>(timeSec) % 60;
	int milliseconds = static_cast<int>((timeSec - static_cast<int>(timeSec)) * 100.0f);

	wchar_t buffer[16];
	swprintf(buffer, 16, L"%02d:%02d:%02d", minutes, seconds, milliseconds);

	m_passedTimeText->SetText(std::wstring(buffer));

	//m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));

	// 배터리 계산
	int killCount = GamePlayManager::GetInstance().GetKillEnemyAmount();
	int& batteryCount = GamePlayManager::GetInstance().batteryCount;

	if (prevKillAmount != killCount) {
		int delta = killCount - prevKillAmount;

		batteryCount = batteryCount + delta > maxBattery ?
			maxBattery : batteryCount + delta;

		prevKillAmount = killCount;
	}

	// 배터리 프로그래스 설정
	if (prevBattery != batteryCount) {
		SetProgress();
		prevBattery = batteryCount;
	}

	// 속도 연동

	float playerSpeed = 0.0f;
	if (WeakObjectPtr<gameObject> player = GetWorld()->FindObjectByTag<gameObject>(L"Player"))
	{
		if (BikeMovementScript* pm = player->GetComponent<BikeMovementScript>())
			playerSpeed = pm->GetCurrentSpeed() * pm->GetSpeedModifierValue();
	}

	float finalSpeed = playerSpeed / 600.0f;

	if (finalSpeed > 1.0f) finalSpeed = 0.999999f;	// 어느정도 허용치를 둠

	m_speedProgress->SetProgress(finalSpeed);

	m_speedText->SetText(playerSpeed);
	m_speedText->m_layer = Define::NormalTextLayer;
}

void StageWidgetScript::Awake()
{
	// 씬 처음 들어오면 배터리 카운트는 0
	GamePlayManager::GetInstance().batteryCount = 0;
}

void StageWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	float soundUISize = 1;
	float soundTabPosX = 450;

	auto pauseButton = m_owner->AddComponent<ButtonComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();
	auto smallClose = m_owner->AddComponent<ButtonComponent>();

	auto toOption = m_owner->AddComponent<ButtonComponent>();
	auto toMain = m_owner->AddComponent<ButtonComponent>();
	auto toRestart = m_owner->AddComponent<ButtonComponent>();
	auto toSelect = m_owner->AddComponent<ButtonComponent>();

	auto bgmPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto bgmMinusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxPlusButton = m_owner->AddComponent<ButtonComponent>();
	auto sfxMinusButton = m_owner->AddComponent<ButtonComponent>();

	auto pauseText = m_owner->AddComponent<TextRenderComponent>();
	auto optionText = m_owner->AddComponent<TextRenderComponent>();
	auto mainText = m_owner->AddComponent<TextRenderComponent>();
	auto restartText = m_owner->AddComponent<TextRenderComponent>();
	auto selectText = m_owner->AddComponent<TextRenderComponent>();

	auto optionTabText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabBGMText = m_owner->AddComponent<TextRenderComponent>();
	auto optionTabSFXText = m_owner->AddComponent<TextRenderComponent>();

	auto UI_Timer = m_owner->AddComponent<SpriteRenderer>();
	auto UI_HP = m_owner->AddComponent<SpriteRenderer>();
	auto UI_Dashboard = m_owner->AddComponent<SpriteRenderer>();

	m_batteryProgress = m_owner->AddComponent<ProgressBarComponent>();
	m_speedProgress = m_owner->AddComponent<ProgressBarComponent>();
	auto bgmControl = m_owner->AddComponent<ProgressBarComponent>();
	auto sfxControl = m_owner->AddComponent<ProgressBarComponent>();

	m_dashboardText = m_owner->AddComponent<TextRenderComponent>();
	m_speedText = m_owner->AddComponent<TextRenderComponent>();

	// ================== Sprite
	auto popUpTab = m_owner->AddComponent<SpriteRenderer>();
	popUpTab->SetDrawType(EDrawType::ScreenSpace);
	//popUpTab->LoadData(L"UI\\UI_PauseTab.png");
	popUpTab->LoadData(L"UI\\UI_Tab.png");	// 텍스트를 넣읗거면 이거로 사용
	popUpTab->SetRelativePosition(FVector2(0, 0));
	popUpTab->m_layer = Define::Disable;
	popUpTab->SetOpacity(0);

	UI_Timer->LoadData(L"UI\\UI_Time.png");
	UI_Timer->SetDrawType(EDrawType::ScreenSpace);
	FVector2 TimerSize = UI_Timer->GetRelativeSize();
	UI_Timer->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(TimerSize, FVector2(0, 0))
		+ FVector2(-820, -SCREEN_HEIGHT / 2.0f + 80));
	UI_Timer->m_layer = Define::HUDLayer;

	UI_Dashboard->LoadData(L"UI\\UI_Dashboard.png");
	UI_Dashboard->SetDrawType(EDrawType::ScreenSpace);
	FVector2 DashboardSize = UI_Dashboard->GetRelativeSize();
	UI_Dashboard->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(DashboardSize, FVector2(0, 0))
		+ FVector2(-670, SCREEN_HEIGHT / 2.0f - 210)
	);
	UI_Dashboard->m_layer = Define::HUDLayer;

	// Progress bar
	m_batteryProgress->SetDrawType(Define::EDrawType::ScreenSpace);
	m_batteryProgress->LoadData(L"UI\\UI_2_Battery.png");
	m_batteryProgress->SetType(EProgressBarType::Linear);
	m_batteryProgress->SetRelativeScale(FVector2(1, 1));
	m_batteryProgress->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_batteryProgress->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(-596, SCREEN_HEIGHT / 2.0f - 169));
	m_batteryProgress->SetProgress(0);
	m_batteryProgress->m_layer = Define::HUDLayer + 10;

	// Progress bar - (원형 프로그레스)
	m_speedProgress->SetDrawType(Define::EDrawType::ScreenSpace);
	m_speedProgress->LoadData(L"UI\\UI_2_Speed.png");
	m_speedProgress->SetType(EProgressBarType::Radial);
	m_speedProgress->SetRelativeScale(FVector2(1, 1));
	m_speedProgress->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(m_speedProgress->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(-SCREEN_WIDTH / 2.0f + 139, SCREEN_HEIGHT / 2.0f - 210));
	m_speedProgress->SetStartAngleDeg(0);
	m_speedProgress->SetProgress(1);
	m_speedProgress->m_layer = Define::HUDLayer + 10;

	// 사운드 관련
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
	soundControl->SetRelativePosition(FVector2(soundTabPosX, 0.f));

	// Progress bar
	bgmControl->SetDrawType(Define::EDrawType::ScreenSpace);
	bgmControl->LoadData(L"UI\\ControlBar.png");
	bgmControl->SetType(EProgressBarType::Linear);
	bgmControl->SetRelativeScale(FVector2(1, 1));
	bgmControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(bgmControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(soundTabPosX + 5, 3));
	bgmControl->SetProgress(bgmVolume);
	bgmControl->m_layer = Define::Disable;

	sfxControl->SetDrawType(Define::EDrawType::ScreenSpace);
	sfxControl->LoadData(L"UI\\ControlBar.png");
	sfxControl->SetType(EProgressBarType::Linear);
	sfxControl->SetRelativeScale(FVector2(1, 1));
	sfxControl->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(sfxControl->GetRelativeSize(), FVector2(0, 0))
		+ FVector2(soundTabPosX + 5, 66));
	sfxControl->SetProgress(sfxVolume);
	sfxControl->m_layer = Define::Disable;

	pauseText->SetFontSize(85.f);
	pauseText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	pauseText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	pauseText->SetText(L"일시정지");
	pauseText->SetColor(FColor::White);
	pauseText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(pauseText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, -200)
	);
	pauseText->m_layer = Define::Disable;

	optionText->SetFontSize(40.f);
	optionText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionText->SetText(L"음량 조절");
	optionText->SetColor(FColor::White);
	optionText->RemoveFromParent();
	toOption->AddChildComponent(optionText);
	optionText->SetRelativePosition(CoordHelper::RatioCoordToScreen(optionText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	optionText->m_layer = Define::Disable;

	optionTabText->SetFontSize(55.0f);
	optionTabText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	optionTabText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	optionTabText->SetText(L"음량 조절");
	optionTabText->SetColor(FColor::White);
	FVector2 optionTabTextRectSize = optionTabText->GetRelativeSize();
	optionTabText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(optionTabTextRectSize, FVector2(-0.5, -0.5))
		+ FVector2(soundTabPosX, -85)
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
		+ FVector2(soundTabPosX, -30)
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
		+ FVector2(soundTabPosX, 35)
	);
	optionTabSFXText->SetRelativeRotation(0);
	optionTabSFXText->m_layer = Define::Disable;

	mainText->SetFontSize(40.f);
	mainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	mainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	mainText->SetText(L"처음 화면으로");
	mainText->SetColor(FColor::White);
	mainText->RemoveFromParent();
	toMain->AddChildComponent(mainText);
	mainText->SetRelativePosition(CoordHelper::RatioCoordToScreen(mainText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	mainText->m_layer = Define::Disable;

	restartText->SetFontSize(40.f);
	restartText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	restartText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	restartText->SetText(L"임무 재시작");
	restartText->SetColor(FColor::White);
	restartText->RemoveFromParent();
	toRestart->AddChildComponent(restartText);
	restartText->SetRelativePosition(CoordHelper::RatioCoordToScreen(restartText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	restartText->m_layer = Define::Disable;

	selectText->SetFontSize(40.f);
	selectText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	selectText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	selectText->SetText(L"임무 선택화면");
	selectText->SetColor(FColor::White);
	selectText->RemoveFromParent();
	toSelect->AddChildComponent(selectText);
	selectText->SetRelativePosition(CoordHelper::RatioCoordToScreen(selectText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	selectText->m_layer = Define::Disable;

	// ========================== pauseButton
	pauseButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_Pause.png");
	pauseButton->LoadData(Define::EButtonState::Release, L"UI\\UI_Pause.png");
	FVector2 pauseSize = pauseButton->GetRelativeSize();
	pauseButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(pauseSize, FVector2(0, 0))
		+ FVector2(900, -SCREEN_HEIGHT / 2.0f + 60)
	);
	pauseButton->m_layer = Define::ButtonLayer;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	closeButton->SetDrawType(EDrawType::ScreenSpace);
	closeButton->SetRelativePosition(FVector2(235, -345));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetActive(false);
	closeButton->m_layer = Define::Disable;

	smallClose->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	smallClose->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	smallClose->SetDrawType(EDrawType::ScreenSpace);
	smallClose->SetRelativePosition(FVector2(680, -130));
	smallClose->SetRelativeScale(FVector2(0.5, 0.5));
	smallClose->SetActive(false);
	smallClose->m_layer = Define::Disable;

	// ======================== toRestart
	toRestart->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toRestart->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toRestart->SetDrawType(EDrawType::ScreenSpace);
	toRestart->SetRelativePosition(FVector2(0, -50));
	toRestart->SetRelativeScale(FVector2(1, 1));
	toRestart->SetActive(false);
	toRestart->m_layer = Define::Disable;

	// ======================== toSelect
	toSelect->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toSelect->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toSelect->SetDrawType(EDrawType::ScreenSpace);
	toSelect->SetRelativePosition(FVector2(0, 50));
	toSelect->SetRelativeScale(FVector2(1, 1));
	toSelect->SetActive(false);
	toSelect->m_layer = Define::Disable;

	// ======================== toOption
	toOption->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toOption->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toOption->SetDrawType(EDrawType::ScreenSpace);
	toOption->SetRelativePosition(FVector2(0, 150));
	toOption->SetRelativeScale(FVector2(1, 1));
	toOption->SetActive(false);
	toOption->m_layer = Define::Disable;
	
	// ======================== toMain
	toMain->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toMain->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toMain->SetDrawType(EDrawType::ScreenSpace);
	toMain->SetRelativePosition(FVector2(0, 250));
	toMain->SetRelativeScale(FVector2(1, 1));
	toMain->SetActive(false);
	toMain->m_layer = Define::Disable;

	// ======================== soundButton
	bgmPlusButton->LoadData(Define::EButtonState::Idle, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Hover, L"UI\\SoundPlus_Idle.png");
	bgmPlusButton->LoadData(Define::EButtonState::Pressed, L"UI\\SoundPlus_Pressed.png");
	bgmPlusButton->LoadData(Define::EButtonState::Release, L"UI\\SoundPlus_Idle.png");
	{
		const float hw = soundControl->GetBitmapSizeX() * soundUISize * 0.5f;
		const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		const float marginX = -350.f;
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
		const float marginX = 560.f;
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
		const float marginX = -350.f;
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
		const float marginX = 560.f;
		const float marginY = 10.7f;
		const float rowY = panelH * 0.25f;
		sfxMinusButton->SetRelativePosition(FVector2(-hw + marginX, rowY - marginY));
	}
	sfxMinusButton->SetActive(false);
	sfxMinusButton->m_layer = Define::Disable;

	// ========================= Delegate
	pauseButton->SetStateAction(Define::EButtonState::Hover, [pauseButton]()
		{
			pauseButton->StartHoverPulse(0.8f, 0.04f);
			pauseButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::HoverLeave, [pauseButton]()
		{
			pauseButton->StopHoverPulse();
			pauseButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::Release, [pauseButton]()
		{
			pauseButton->StopHoverPulse();
			pauseButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	pauseButton->SetStateAction(Define::EButtonState::Pressed, [this,
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		sound->PauseByType(SoundType::BGM);

		GamePlayManager::GetInstance().PauseGame();

		m_isPaused = !m_isPaused;

		pauseButton->SetActive(false);
		popUpTab->m_layer = Define::PopupLayer;
		popUpTab->SetOpacity(1);

		pauseText->m_layer = Define::PopupTextLayer;
		closeButton->SetActive(true);
		closeButton->m_layer = Define::PopupButtonLayer;

		toMain->SetActive(true);
		toMain->m_layer = Define::PopupButtonLayer;
		mainText->m_layer = Define::PopupTextLayer;

		toRestart->SetActive(true);
		toRestart->m_layer = Define::PopupButtonLayer;
		restartText->m_layer = Define::PopupTextLayer;

		toOption->SetActive(true);
		toOption->m_layer = Define::PopupButtonLayer;
		optionText->m_layer = Define::PopupTextLayer;

		toSelect->SetActive(true);
		toSelect->m_layer = Define::PopupButtonLayer;
		selectText->m_layer = Define::PopupTextLayer;
		});

	//closeButton->SetStateAction(Define::EButtonState::Hover, [closeButton]()
	//	{
	//		closeButton->StartHoverPulse(0.8f, 0.04f);
	//		closeButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	//	});

	//closeButton->SetStateAction(Define::EButtonState::HoverLeave, [closeButton]()
	//	{
	//		closeButton->StopHoverPulse();
	//		closeButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	//	});

	//closeButton->SetStateAction(Define::EButtonState::Release, [closeButton]()
	//	{
	//		closeButton->StopHoverPulse();
	//		closeButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	//	});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [this,
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText, soundControl,
		bgmControl, sfxControl, bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton,
		optionTabText, optionTabBGMText, optionTabSFXText
	] {
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		sound->ResumeByType(SoundType::BGM);
		
		GamePlayManager::GetInstance().ResumeGame();

		m_isPaused = !m_isPaused;

		pauseText->m_layer = Define::Disable;
		pauseButton->SetActive(true);
		popUpTab->m_layer = Define::Disable;
		popUpTab->SetOpacity(0);

		closeButton->SetActive(false);
		closeButton->m_layer = Define::Disable;

		toMain->SetActive(false);
		toMain->m_layer = Define::Disable;
		mainText->m_layer = Define::Disable;

		toRestart->SetActive(false);
		toRestart->m_layer = Define::Disable;
		restartText->m_layer = Define::Disable;

		toOption->SetActive(false);
		toOption->m_layer = Define::Disable;
		optionText->m_layer = Define::Disable;

		toSelect->SetActive(false);
		toSelect->m_layer = Define::Disable;
		selectText->m_layer = Define::Disable;

		optionTabText->m_layer = Define::Disable;
		optionTabBGMText->m_layer = Define::Disable;
		optionTabSFXText->m_layer = Define::Disable;

		soundControl->m_layer = Define::Disable;
		bgmControl->m_layer = Define::Disable;
		sfxControl->m_layer = Define::Disable;

		bgmPlusButton->m_layer = Define::Disable;
		bgmPlusButton->SetActive(false);
		bgmMinusButton->m_layer = Define::Disable;
		bgmMinusButton->SetActive(false);
		sfxPlusButton->m_layer = Define::Disable;
		sfxPlusButton->SetActive(false);
		sfxMinusButton->m_layer = Define::Disable;
		sfxMinusButton->SetActive(false);
		});

	// bgmPlusButton Hover 효과 (시안색 글로우)
	bgmPlusButton->SetStateAction(Define::EButtonState::Hover, [bgmPlusButton]()
		{
			bgmPlusButton->StartHoverPulse(0.8f, 0.04f);
			bgmPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	bgmPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmPlusButton]()
		{
			bgmPlusButton->StopHoverPulse();
			bgmPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	bgmPlusButton->SetStateAction(Define::EButtonState::Release, [bgmPlusButton]()
		{
			bgmPlusButton->StopHoverPulse();
			bgmPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// bgmMinusButton Hover 효과 (시안색 글로우)
	bgmMinusButton->SetStateAction(Define::EButtonState::Hover, [bgmMinusButton]()
		{
			bgmMinusButton->StartHoverPulse(0.8f, 0.04f);
			bgmMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [bgmMinusButton]()
		{
			bgmMinusButton->StopHoverPulse();
			bgmMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::Release, [bgmMinusButton]()
		{
			bgmMinusButton->StopHoverPulse();
			bgmMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// sfxPlusButton Hover 효과 (시안색 글로우)
	sfxPlusButton->SetStateAction(Define::EButtonState::Hover, [sfxPlusButton]()
		{
			sfxPlusButton->StartHoverPulse(0.8f, 0.04f);
			sfxPlusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxPlusButton]()
		{
			sfxPlusButton->StopHoverPulse();
			sfxPlusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::Release, [sfxPlusButton]()
		{
			sfxPlusButton->StopHoverPulse();
			sfxPlusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	// sfxMinusButton Hover 효과 (시안색 글로우)
	sfxMinusButton->SetStateAction(Define::EButtonState::Hover, [sfxMinusButton]()
		{
			sfxMinusButton->StartHoverPulse(0.8f, 0.04f);
			sfxMinusButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::HoverLeave, [sfxMinusButton]()
		{
			sfxMinusButton->StopHoverPulse();
			sfxMinusButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Release, [sfxMinusButton]()
		{
			sfxMinusButton->StopHoverPulse();
			sfxMinusButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});


	bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl] {
		
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol += 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);

		bgmControl->SetProgress(vol);
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, bgmControl] {
		
		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");
		
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol -= 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);

		bgmControl->SetProgress(vol);
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [sound, sfxControl] {

		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx += 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);

		sfxControl->SetProgress(sfx);
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [sound, sfxControl] {

		sound->StopByName(L"UISound");
		sound->PlayByName(L"UISound");

		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx -= 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);

		sfxControl->SetProgress(sfx);
		});

	// 효과 (주황색 글로우)
	toSelect->SetStateAction(Define::EButtonState::Hover, [toSelect]()
		{
			toSelect->StartHoverPulse(0.8f, 0.04f);
			toSelect->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::HoverLeave, [toSelect]()
		{
			toSelect->StopHoverPulse();
			toSelect->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::Release, [toSelect]()
		{
			toSelect->StopHoverPulse();
			toSelect->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toSelect->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"SelectScene");
		});

	// 효과 (주황색 글로우)
	toOption->SetStateAction(Define::EButtonState::Hover, [toOption]()
		{
			toOption->StartHoverPulse(0.8f, 0.04f);
			toOption->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::HoverLeave, [toOption]()
		{
			toOption->StopHoverPulse();
			toOption->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::Release, [toOption]()
		{
			toOption->StopHoverPulse();
			toOption->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toOption->SetStateAction(Define::EButtonState::Pressed, [sound,
		soundControl, bgmControl, sfxControl,
		bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton,
		pauseButton, closeButton, toOption, toMain, toRestart, toSelect, smallClose,
		optionTabText, optionTabBGMText, optionTabSFXText
	] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

		pauseButton->SetActive(false);
		closeButton->SetActive(false);
		toOption->SetActive(false);
		toMain->SetActive(false);
		toRestart->SetActive(false);
		toSelect->SetActive(false);

		smallClose->SetActive(true);
		smallClose->m_layer = PopupButtonLayer;

		optionTabText->m_layer = Define::PopupTextLayer;
		optionTabBGMText->m_layer = Define::PopupTextLayer;
		optionTabSFXText->m_layer = Define::PopupTextLayer;

		soundControl->m_layer = Define::PopupPopLayer;
		bgmControl->m_layer = Define::PopupObjectLayer;
		sfxControl->m_layer = Define::PopupObjectLayer;

		bgmPlusButton->m_layer = Define::PopupButtonLayer;
		bgmPlusButton->SetActive(true);
		bgmMinusButton->m_layer = Define::PopupButtonLayer;
		bgmMinusButton->SetActive(true);
		sfxPlusButton->m_layer = Define::PopupButtonLayer;
		sfxPlusButton->SetActive(true);
		sfxMinusButton->m_layer = Define::PopupButtonLayer;
		sfxMinusButton->SetActive(true);
		});

	smallClose->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, toOption, toMain, toRestart, toSelect,
		smallClose, soundControl, bgmControl, sfxControl, bgmPlusButton, bgmMinusButton,
		sfxPlusButton, sfxMinusButton, sound, optionTabText, optionTabBGMText, optionTabSFXText

	] {
			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");
			
		pauseButton->SetActive(true);
		closeButton->SetActive(true);
		toOption->SetActive(true);
		toMain->SetActive(true);
		toRestart->SetActive(true);
		toSelect->SetActive(true);

		smallClose->SetActive(false);
		smallClose->m_layer = Define::Disable;

		optionTabText->m_layer = Define::Disable;
		optionTabBGMText->m_layer = Define::Disable;
		optionTabSFXText->m_layer = Define::Disable;

		soundControl->m_layer = Define::Disable;
		bgmControl->m_layer = Define::Disable;
		sfxControl->m_layer = Define::Disable;

		bgmPlusButton->m_layer = Define::Disable;
		bgmPlusButton->SetActive(false);
		bgmMinusButton->m_layer = Define::Disable;
		bgmMinusButton->SetActive(false);
		sfxPlusButton->m_layer = Define::Disable;
		sfxPlusButton->SetActive(false);
		sfxMinusButton->m_layer = Define::Disable;
		sfxMinusButton->SetActive(false);

		});

	// 효과 (주황색 글로우)
	toRestart->SetStateAction(Define::EButtonState::Hover, [toRestart]()
		{
			toRestart->StartHoverPulse(0.8f, 0.04f);
			toRestart->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::HoverLeave, [toRestart]()
		{
			toRestart->StopHoverPulse();
			toRestart->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::Release, [toRestart]()
		{
			toRestart->StopHoverPulse();
			toRestart->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toRestart->SetStateAction(Define::EButtonState::Pressed, [weak = WeakFromThis<StageWidgetScript>(), sound,
		pauseText, pauseButton, popUpTab, closeButton, toMain, mainText,
		toRestart, restartText, toOption, optionText, toSelect, selectText
	] {
		sound->PlayByName(L"UISound");

		// TODO : 재시작 코드
		if (weak.expired())return;
		std::wstring sceneName = weak->GetWorld()->GetName();
		SceneManager::GetInstance().ChangeScene(sceneName);

		pauseText->m_layer = Define::Disable;
		pauseButton->SetActive(true);
		popUpTab->m_layer = Define::Disable;
		popUpTab->SetOpacity(0);

		closeButton->SetActive(false);
		closeButton->m_layer = Define::Disable;

		toMain->SetActive(false);
		toMain->m_layer = Define::Disable;
		mainText->m_layer = Define::Disable;

		toRestart->SetActive(false);
		toRestart->m_layer = Define::Disable;
		restartText->m_layer = Define::Disable;

		toOption->SetActive(false);
		toOption->m_layer = Define::Disable;
		optionText->m_layer = Define::Disable;

		toSelect->SetActive(false);
		toSelect->m_layer = Define::Disable;
		selectText->m_layer = Define::Disable;
		});

	// 효과 (주황색 글로우)
	toMain->SetStateAction(Define::EButtonState::Hover, [toMain]()
		{
			toMain->StartHoverPulse(0.8f, 0.04f);
			toMain->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::HoverLeave, [toMain]()
		{
			toMain->StopHoverPulse();
			toMain->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::Release, [toMain]()
		{
			toMain->StopHoverPulse();
			toMain->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toMain->SetStateAction(Define::EButtonState::Pressed, [] {

		SceneManager::ChangeScene(L"TitleScene");
		});

	m_passedTimeText = m_owner->AddComponent<TextRenderComponent>();
	m_passedTimeText->SetText(std::wstring());
	m_passedTimeText->SetFontSize(45.0f);
	m_passedTimeText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_passedTimeText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_passedTimeText->SetTextAlignment(ETextFormat::TopLeft);
	m_passedTimeText->SetRelativePosition(FVector2(-SCREEN_WIDTH / 2.0f + 65, -SCREEN_HEIGHT / 2.0f + 80));
	m_passedTimeText->SetColor(FColor(0, 234, 255, 255));
	m_passedTimeText->m_layer = Define::NormalTextLayer;

	//m_killEnemyText = owner->AddComponent<TextRenderComponent>();
	//m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));
	//m_killEnemyText->SetTextAlignment(ETextFormat::TopLeft);
	//m_killEnemyText->SetRelativePosition(FVector2(20, 80));
	//m_killEnemyText->SetFontSize(32.0f);
	//m_killEnemyText->SetColor(FColor::Gold);
	//m_killEnemyText->m_layer = Define::NormalTextLayer;

	auto input = m_owner->AddComponent<InputComponent>();
	input->SetAction(input->GetHandle(), [this,
		sound, pauseButton, popUpTab, pauseText, closeButton, toMain, mainText,
		toRestart, restartText, toOption, optionText, toSelect, selectText,
		optionTabText, optionTabBGMText, optionTabSFXText,
		soundControl, bgmControl, sfxControl,bgmPlusButton,bgmMinusButton,sfxPlusButton,sfxMinusButton,
		smallClose
	] {
		if (!m_isPaused && Input::IsKeyPressed(VK_ESCAPE))
		{
			m_isPaused = !m_isPaused;

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			GamePlayManager::GetInstance().PauseGame();

			sound->PauseByType(SoundType::BGM);

			pauseButton->SetActive(false);
			popUpTab->m_layer = Define::PopupLayer;
			popUpTab->SetOpacity(1);

			pauseText->m_layer = Define::PopupTextLayer;
			closeButton->SetActive(true);
			closeButton->m_layer = Define::PopupButtonLayer;

			toMain->SetActive(true);
			toMain->m_layer = Define::PopupButtonLayer;
			mainText->m_layer = Define::PopupTextLayer;

			toRestart->SetActive(true);
			toRestart->m_layer = Define::PopupButtonLayer;
			restartText->m_layer = Define::PopupTextLayer;

			toOption->SetActive(true);
			toOption->m_layer = Define::PopupButtonLayer;
			optionText->m_layer = Define::PopupTextLayer;

			toSelect->SetActive(true);
			toSelect->m_layer = Define::PopupButtonLayer;
			selectText->m_layer = Define::PopupTextLayer;
		}
		else if (m_isPaused && Input::IsKeyPressed(VK_ESCAPE))
		{
			m_isPaused = !m_isPaused;

			sound->StopByName(L"UISound");
			sound->PlayByName(L"UISound");

			GamePlayManager::GetInstance().ResumeGame();

			sound->ResumeByType(SoundType::BGM);

			pauseText->m_layer = Define::Disable;
			pauseButton->SetActive(true);
			popUpTab->m_layer = Define::Disable;
			popUpTab->SetOpacity(0);

			closeButton->SetActive(false);
			closeButton->m_layer = Define::Disable;

			toMain->SetActive(false);
			toMain->m_layer = Define::Disable;
			mainText->m_layer = Define::Disable;

			toRestart->SetActive(false);
			toRestart->m_layer = Define::Disable;
			restartText->m_layer = Define::Disable;

			toOption->SetActive(false);
			toOption->m_layer = Define::Disable;
			optionText->m_layer = Define::Disable;

			toSelect->SetActive(false);
			toSelect->m_layer = Define::Disable;
			selectText->m_layer = Define::Disable;

			optionTabText->m_layer = Define::Disable;
			optionTabBGMText->m_layer = Define::Disable;
			optionTabSFXText->m_layer = Define::Disable;

			soundControl->m_layer = Define::Disable;
			bgmControl->m_layer = Define::Disable;
			sfxControl->m_layer = Define::Disable;

			bgmPlusButton->m_layer = Define::Disable;
			bgmPlusButton->SetActive(false);
			bgmMinusButton->m_layer = Define::Disable;
			bgmMinusButton->SetActive(false);
			sfxPlusButton->m_layer = Define::Disable;
			sfxPlusButton->SetActive(false);
			sfxMinusButton->m_layer = Define::Disable;
			sfxMinusButton->SetActive(false);

			smallClose->SetActive(false);
			smallClose->m_layer = Define::Disable;
		}
		});
}

void StageWidgetScript::OnEnd()
{
}

void StageWidgetScript::OnDestroy()
{
}

void StageWidgetScript::SetProgress()
{
	float bProgress = (float)GamePlayManager::GetInstance().batteryCount / (float)maxBattery;
	bProgress = bProgress > 1.0f ? 1.0f : bProgress;	// Battery : 0~1
	m_batteryProgress->SetProgress(bProgress);
}
