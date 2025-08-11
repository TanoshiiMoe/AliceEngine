#include "StageWidgetScript.h"

#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Math/Transform.h>
#include <Helpers/CoordHelper.h>

#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/SpriteRenderer.h>
#include <Component/TextRenderComponent.h>
#include <Component/ButtonComponent.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>

#include <GameManager/GamePlayManager.h>

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
}

void StageWidgetScript::Awake()
{
}

void StageWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	float soundUISize = 1;

	auto pauseButton = m_owner->AddComponent<ButtonComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

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

	auto UI_Timer = m_owner->AddComponent<SpriteRenderer>();
	auto UI_HP = m_owner->AddComponent<SpriteRenderer>();
	auto UI_Dashboard = m_owner->AddComponent<SpriteRenderer>();
	auto UI_Speed = m_owner->AddComponent<SpriteRenderer>();
	auto UI_Battery = m_owner->AddComponent<SpriteRenderer>();

	auto DashboardText = m_owner->AddComponent<TextRenderComponent>();
	auto SpeedText = m_owner->AddComponent<TextRenderComponent>();

	auto sound = m_owner->AddComponent<AudioComponent>(L"UISound");
	sound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);

	auto popUpTab = m_owner->AddComponent<SpriteRenderer>();
	popUpTab->SetDrawType(EDrawType::ScreenSpace);
	//popUpTab->LoadData(L"UI\\UI_PauseTab.png");
	popUpTab->LoadData(L"UI\\UI_Tab.png");	// 텍스트를 넣읗거면 이거로 사용
	popUpTab->SetRelativePosition(FVector2(0, 0));
	popUpTab->m_layer = -1000;
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

	// 사운드 관련
	auto bgmObj = GetWorld()->FindObjectByName<gameObject>(L"Sound");
	if (!bgmObj) return;
	auto bgm = bgmObj->GetComponent<AudioComponent>();

	float bgmVolume = AudioManager::GetInstance().GetBGMVolume();
	float sfxVolume = AudioManager::GetInstance().GetSFXVolume();

	auto soundControl = m_owner->AddComponent<SpriteRenderer>();
	soundControl->LoadData(L"UI\\UI_SoundControl.png");
	soundControl->m_layer = Define::Disable;
	soundControl->SetRelativeScale(soundUISize);
	soundControl->SetDrawType(EDrawType::ScreenSpace);
	// 중앙 배치
	soundControl->SetRelativePosition(FVector2(0.f, 0.f));

	auto bgmControl = m_owner->AddComponent<SpriteRenderer>();
	bgmControl->LoadData(L"UI\\ControlBar.png");
	bgmControl->m_layer = Define::Disable;
	bgmControl->SetDrawType(EDrawType::ScreenSpace);
	// 게이지는 슬라이스로 표현하고, 스케일은 UI 전체 배율만 적용
	bgmControl->SetRelativeScale(FVector2(soundUISize, soundUISize));
	bgmControl->SetSlice(0, 0, bgmControl->GetBitmapSizeX() * bgmVolume, bgmControl->GetBitmapSizeY());
	// 중앙 기준 위쪽 라인 위치
	{
		const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		bgmControl->SetRelativePosition(FVector2(0.f, -panelH * 0.25f));
	}

	auto sfxControl = m_owner->AddComponent<SpriteRenderer>();
	sfxControl->LoadData(L"UI\\ControlBar.png");
	sfxControl->m_layer = Define::Disable;
	sfxControl->SetDrawType(EDrawType::ScreenSpace);
	// 게이지는 슬라이스로 표현하고, 스케일은 UI 전체 배율만 적용
	sfxControl->SetRelativeScale(FVector2(soundUISize, soundUISize));

	sfxControl->SetSlice(0, 0, sfxControl->GetBitmapSizeX() * sfxVolume, sfxControl->GetBitmapSizeY());
	// 중앙 기준 아래쪽 라인 위치
	{
		const float panelH = soundControl->GetBitmapSizeY() * soundUISize;
		sfxControl->SetRelativePosition(FVector2(0.f, panelH * 0.25f));
	}

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

	// ========================= Delegate
	pauseButton->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText
	] {
		sound->PlayByName(L"UISound", 0.45);

		GamePlayManager::GetInstance().PauseGame();

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

	closeButton->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText,
		optionText, mainText, restartText, selectText, soundControl,
		bgmControl, sfxControl, bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton
	] {
		sound->PlayByName(L"UISound", 0.45);
		
		//GamePlayManager::GetInstance().ResumeGame();

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

	bgmPlusButton->SetStateAction(Define::EButtonState::Pressed, [] {
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol += 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);
		});

	bgmMinusButton->SetStateAction(Define::EButtonState::Pressed, [] {
		float vol = AudioManager::GetInstance().GetBGMVolume();
		vol -= 0.1f;
		AudioManager::GetInstance().SetBGMVolume(vol);
		});

	sfxPlusButton->SetStateAction(Define::EButtonState::Pressed, [] {
		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx += 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);
		});

	sfxMinusButton->SetStateAction(Define::EButtonState::Pressed, [] {
		float sfx = AudioManager::GetInstance().GetSFXVolume();
		sfx -= 0.1f;
		AudioManager::GetInstance().SetSFXVolume(sfx);
		});

	toMain->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"TitleScene");
		});

	toSelect->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"SelectScene");
		});

	toOption->SetStateAction(Define::EButtonState::Pressed, [sound,
		soundControl, bgmControl, sfxControl,
		bgmPlusButton, bgmMinusButton, sfxPlusButton, sfxMinusButton
	] {
		sound->PlayByName(L"UISound", 0.45);

		soundControl->m_layer = Define::PopupLayer;
		bgmControl->m_layer = Define::PopupButtonLayer;
		sfxControl->m_layer = Define::PopupButtonLayer;

		bgmPlusButton->m_layer = Define::PopupButtonLayer;
		bgmPlusButton->SetActive(true);
		bgmMinusButton->m_layer = Define::PopupButtonLayer;
		bgmMinusButton->SetActive(true);
		sfxPlusButton->m_layer = Define::PopupButtonLayer;
		sfxPlusButton->SetActive(true);
		sfxMinusButton->m_layer = Define::PopupButtonLayer;
		sfxMinusButton->SetActive(true);
		});

	toRestart->SetStateAction(Define::EButtonState::Pressed, [sound,
		pauseText, pauseButton, popUpTab, closeButton, toMain, mainText,
		toRestart, restartText, toOption, optionText, toSelect, selectText
	] {
		sound->PlayByName(L"UISound", 0.45);
		// 재시작 하는 코드 필요? 재시작인지 재개인지 이거도 구분해야될듯
		GamePlayManager::GetInstance().ResumeGame();

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
}

void StageWidgetScript::OnEnd()
{
}

void StageWidgetScript::OnDestroy()
{
}
