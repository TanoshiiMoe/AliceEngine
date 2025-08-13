#include "GameOverWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <System/ScriptSystem.h>

#include <Helpers/CoordHelper.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>

#include <UI/UIImage.h>
#include <UI/UIText.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>

#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>

void GameOverWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void GameOverWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void GameOverWidgetScript::Awake()
{
}

void GameOverWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	auto background = m_owner->AddComponent<SpriteRenderer>();

	auto errorPopup = m_owner->AddComponent<SpriteRenderer>();

	auto toRestartButton = m_owner->AddComponent<ButtonComponent>();
	auto toMainButton = m_owner->AddComponent<ButtonComponent>();

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toRestartText = m_owner->AddComponent<TextRenderComponent>();

	// ==================== //
	background->LoadData(L"UI\\UI_Score_FAIL.png");
	background->SetDrawType(EDrawType::ScreenSpace);
	background->m_layer = Define::NormalUILayer;

	errorPopup->LoadData(L"UI\\UI_ERROR_Popup.png");
	errorPopup->SetDrawType(EDrawType::ScreenSpace);
	errorPopup->m_layer = Define::PopupLayer;

	toRestartText->SetFontSize(45.f);
	toRestartText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toRestartText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toRestartText->SetText(L"예");
	toRestartText->SetColor(FColor::Black);
	toRestartText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toRestartText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toRestartText->m_layer = Define::PopupTextLayer;
	toRestartText->RemoveFromParent();
	toRestartButton->AddChildComponent(toRestartText);

	toMainText->SetFontSize(45.f);
	toMainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toMainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toMainText->SetText(L"아니오");
	toMainText->SetColor(FColor::Black);
	toMainText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toMainText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toMainText->m_layer = Define::PopupTextLayer;
	toMainText->RemoveFromParent();
	toMainButton->AddChildComponent(toMainText);

	// ==================== Button
	toRestartButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_ERRORButton.png");
	toRestartButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_ERRORButton.png");
	toRestartButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ERRORButton.png");
	toRestartButton->LoadData(Define::EButtonState::Release, L"UI\\UI_ERRORButton.png");
	toRestartButton->m_layer = Define::PopupButtonLayer;
	toRestartButton->SetDrawType(EDrawType::ScreenSpace);
	toRestartButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toRestartButton->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(-180 + 100, 100));

	toMainButton->LoadData(Define::EButtonState::Idle, L"UI\\UI_ERRORButton.png");
	toMainButton->LoadData(Define::EButtonState::Hover, L"UI\\UI_ERRORButton.png");
	toMainButton->LoadData(Define::EButtonState::Pressed, L"UI\\UI_ERRORButton.png");
	toMainButton->LoadData(Define::EButtonState::Release, L"UI\\UI_ERRORButton.png");
	toMainButton->SetDrawType(EDrawType::ScreenSpace);
	toMainButton->m_layer = Define::PopupButtonLayer;
	toMainButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toMainButton->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(180, 100));

	// ==================== Delegate
	toRestartButton->SetStateAction(Define::EButtonState::Hover, [toRestartButton]()
		{
			toRestartButton->StartHoverPulse(0.8f, 0.04f);
			toRestartButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	toRestartButton->SetStateAction(Define::EButtonState::HoverLeave, [toRestartButton]()
		{
			toRestartButton->StopHoverPulse();
			toRestartButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	toRestartButton->SetStateAction(Define::EButtonState::Release, [toRestartButton]()
		{
			toRestartButton->StopHoverPulse();
			toRestartButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	toRestartButton->SetStateAction(Define::EButtonState::Pressed, [] {
		std::wstring prevScene = SceneManager::GetPrevSceneName();
		SceneManager::ChangeScene(prevScene);
		});

	toMainButton->SetStateAction(Define::EButtonState::Hover, [toMainButton]()
		{
			toMainButton->StartHoverPulse(0.8f, 0.04f);
			toMainButton->StartEffectAnimation(0.3f, 1.2f, FColor(0, 234, 255, 255));
		});

	toMainButton->SetStateAction(Define::EButtonState::HoverLeave, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.2f, 0.0f, FColor(0, 234, 255, 255));
		});

	toMainButton->SetStateAction(Define::EButtonState::Release, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.1f, 0.0f, FColor(0, 234, 255, 255));
		});

	toMainButton->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"TitleScene");
		});

	//TimerManager::GetInstance().ClearTimer(m_fadeHandle);
	//TimerManager::GetInstance().SetTimer(
	//	m_fadeHandle,
	//	[errorPopup]() mutable {
	//		float scale = 0.0f;

	//		scale += 0.1f * TimerManager::GetInstance().unscaledDeltaTime;

	//		if (scale > 1.0f) scale = 1.0f;

	//		errorPopup->SetRelativeScale(scale);


	//	},
	//	1.0f,
	//	false,
	//	1.0f
	//	);
}

void GameOverWidgetScript::OnEnd()
{
}

void GameOverWidgetScript::OnDestroy()
{
}
