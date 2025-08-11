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
	m_passedTimeText->SetText(L"<지난 시간> " + std::to_wstring(GamePlayManager::GetInstance().GetPassedTime()));
	m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));
}

void StageWidgetScript::Awake()
{
}

void StageWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	auto pauseButton = m_owner->AddComponent<ButtonComponent>();
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	auto toOption = m_owner->AddComponent<ButtonComponent>();
	auto toMain = m_owner->AddComponent<ButtonComponent>();
	auto toRestart = m_owner->AddComponent<ButtonComponent>();
	auto toSelect = m_owner->AddComponent<ButtonComponent>();

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

	UI_Timer->LoadData(L"UI\\UI_Time.png");
	UI_Timer->SetDrawType(EDrawType::ScreenSpace);
	FVector2 TimerSize = UI_Timer->GetRelativeSize();
	UI_Timer->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(TimerSize, FVector2(0, 0))
		+ FVector2(-820, -SCREEN_HEIGHT / 2.0f + 80));
	UI_Timer->m_layer = 1000;

	UI_Dashboard->LoadData(L"UI\\UI_Dashboard.png");
	UI_Dashboard->SetDrawType(EDrawType::ScreenSpace);
	FVector2 DashboardSize = UI_Dashboard->GetRelativeSize();
	UI_Dashboard->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(DashboardSize, FVector2(0, 0))
		+ FVector2(-670, SCREEN_HEIGHT / 2.0f - 210)
	);
	UI_Dashboard->m_layer = 1000;

	pauseText->SetFontSize(75.f);
	pauseText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	pauseText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	pauseText->SetText(L"일시정지");
	pauseText->SetColor(FColor::White);
	pauseText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(pauseText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, -250)
	);
	pauseText->m_layer = -2000;

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
	pauseButton->m_layer = 1000;

	// ======================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Close.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Close.png");
	closeButton->SetDrawType(EDrawType::ScreenSpace);
	closeButton->SetRelativePosition(FVector2(235, -345));
	closeButton->SetRelativeScale(FVector2(1, 1));
	closeButton->SetActive(false);
	closeButton->m_layer = -1000;

	// ======================== toOption
	toOption->LoadData(Define::EButtonState::Idle, L"UI\\UI_ToOption.png");
	toOption->LoadData(Define::EButtonState::Hover, L"UI\\UI_ToOption.png");
	toOption->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ToOption.png");
	toOption->LoadData(Define::EButtonState::Release, L"UI\\UI_ToOption.png");
	toOption->SetDrawType(EDrawType::ScreenSpace);
	toOption->SetRelativePosition(FVector2(0, 0));
	toOption->SetRelativeScale(FVector2(1, 1));
	toOption->SetActive(false);
	toOption->m_layer = -1000;

	// ======================== toMain
	toMain->LoadData(Define::EButtonState::Idle, L"UI\\UI_ToMain.png");
	toMain->LoadData(Define::EButtonState::Hover, L"UI\\UI_ToMain.png");
	toMain->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ToMain.png");
	toMain->LoadData(Define::EButtonState::Release, L"UI\\UI_ToMain.png");
	toMain->SetDrawType(EDrawType::ScreenSpace);
	toMain->SetRelativePosition(FVector2(0, 100));
	toMain->SetRelativeScale(FVector2(1, 1));
	toMain->SetActive(false);
	toMain->m_layer = -1000;

	// ======================== toRestart
	toRestart->LoadData(Define::EButtonState::Idle, L"UI\\UI_ToRestart.png");
	toRestart->LoadData(Define::EButtonState::Hover, L"UI\\UI_ToRestart.png");
	toRestart->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ToRestart.png");
	toRestart->LoadData(Define::EButtonState::Release, L"UI\\UI_ToRestart.png");
	toRestart->SetDrawType(EDrawType::ScreenSpace);
	toRestart->SetRelativePosition(FVector2(0, 200));
	toRestart->SetRelativeScale(FVector2(1, 1));
	toRestart->SetActive(false);
	toRestart->m_layer = -1000;

	// ======================== toSelect
	toSelect->LoadData(Define::EButtonState::Idle, L"UI\\UI_ToSelect.png");
	toSelect->LoadData(Define::EButtonState::Hover, L"UI\\UI_ToSelect.png");
	toSelect->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ToSelect.png");
	toSelect->LoadData(Define::EButtonState::Release, L"UI\\UI_ToSelect.png");
	toSelect->SetDrawType(EDrawType::ScreenSpace);
	toSelect->SetRelativePosition(FVector2(0, 300));
	toSelect->SetRelativeScale(FVector2(1, 1));
	toSelect->SetActive(false);
	toSelect->m_layer = -1000;

	// ========================= Delegate
	pauseButton->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText
	] {
			sound->PlayByName(L"UISound", 0.45);

			GamePlayManager::GetInstance().PauseGame();

			pauseButton->SetActive(false);
			popUpTab->m_layer = 2001;

			pauseText->m_layer = 2002;
			closeButton->SetActive(true);
			closeButton->m_layer = 2002;

			toMain->SetActive(true);
			toMain->m_layer = 2002;

			toRestart->SetActive(true);
			toRestart->m_layer = 2002;

			toOption->SetActive(true);
			toOption->m_layer = 2002;

			toSelect->SetActive(true);
			toSelect->m_layer = 2002;
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [
		pauseButton, closeButton, popUpTab,
		toMain, toOption, toRestart, toSelect, sound, pauseText
	] {
			sound->PlayByName(L"UISound", 0.45);

			GamePlayManager::GetInstance().ResumeGame();

			pauseText->m_layer = -2000;
			pauseButton->SetActive(true);
			popUpTab->m_layer = -2000;
			closeButton->SetActive(false);
			closeButton->m_layer = -2000;

			toMain->SetActive(false);
			toMain->m_layer = -2000;

			toRestart->SetActive(false);
			toRestart->m_layer = -2000;

			toOption->SetActive(false);
			toOption->m_layer = -2000;

			toSelect->SetActive(false);
			toSelect->m_layer = -2000;
		});

	toMain->SetStateAction(Define::EButtonState::Pressed, [] {

		SceneManager::ChangeScene(L"TitleScene");
		});


	m_passedTimeText = m_owner->AddComponent<TextRenderComponent>();
	m_passedTimeText->SetText(L"<지난 시간> " + std::to_wstring(GamePlayManager::GetInstance().GetPassedTime()));
	m_passedTimeText->SetTextAlignment(ETextFormat::TopLeft);
	m_passedTimeText->SetRelativePosition(FVector2(20, 40));
	m_passedTimeText->SetFontSize(32.0f);
	m_passedTimeText->SetColor(FColor::Gold);

	m_killEnemyText = owner->AddComponent<TextRenderComponent>();
	m_killEnemyText->SetText(L"<죽인 적수> " + std::to_wstring(GamePlayManager::GetInstance().GetKillEnemyAmount()));
	m_killEnemyText->SetTextAlignment(ETextFormat::TopLeft);
	m_killEnemyText->SetRelativePosition(FVector2(20, 80));
	m_killEnemyText->SetFontSize(32.0f);
	m_killEnemyText->SetColor(FColor::Gold);
}

void StageWidgetScript::OnEnd()
{
}

void StageWidgetScript::OnDestroy()
{
}
