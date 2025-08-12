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

	auto gameOverText = m_owner->AddComponent<TextRenderComponent>();
	auto toRestartButton = m_owner->AddComponent<ButtonComponent>();
	auto toMainButton = m_owner->AddComponent<ButtonComponent>();

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toRestartText = m_owner->AddComponent<TextRenderComponent>();

	gameOverText->SetFontSize(70.f);
	gameOverText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	gameOverText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	gameOverText->SetText(L"...스러지다");
	gameOverText->SetColor(FColor(242, 207, 238, 255));
	gameOverText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(gameOverText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(300, -300));
	gameOverText->m_layer = Define::NormalTextLayer;

	toRestartText->SetFontSize(45.f);
	toRestartText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toRestartText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toRestartText->SetText(L"임무 재도전");
	toRestartText->SetColor(FColor::White);
	toRestartText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toRestartText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toRestartText->m_layer = Define::ButtonTextLayer;
	toRestartText->RemoveFromParent();
	toRestartButton->AddChildComponent(toRestartText);

	toMainText->SetFontSize(45.f);
	toMainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toMainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toMainText->SetText(L"처음 화면으로");
	toMainText->SetColor(FColor::White);
	toMainText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toMainText->GetRelativeSize(), FVector2(-0.5, -0.5))
		+ FVector2(0, 0));
	toMainText->m_layer = Define::ButtonTextLayer;
	toMainText->RemoveFromParent();
	toMainButton->AddChildComponent(toMainText);

	// ==================== Button
	toRestartButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toRestartButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toRestartButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toRestartButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toRestartButton->m_layer = Define::ButtonLayer;
	toRestartButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toRestartButton->GetRelativeSize(), FVector2(1, 0))
		+ FVector2(400, 0));

	toMainButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toMainButton->m_layer = Define::ButtonLayer;
	toMainButton->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(toMainButton->GetRelativeSize(), FVector2(1, 0))
		+ FVector2(400, 100));

	// ==================== Delegate
	toRestartButton->SetStateAction(Define::EButtonState::Hover, [toRestartButton]()
		{
			toRestartButton->StartHoverPulse(0.8f, 0.04f);
			toRestartButton->StartEffectAnimation(0.3f, 1.2f, FColor::White);
		});

	toRestartButton->SetStateAction(Define::EButtonState::HoverLeave, [toRestartButton]()
		{
			toRestartButton->StopHoverPulse();
			toRestartButton->StartEffectAnimation(0.2f, 0.0f, FColor::White);
		});

	toRestartButton->SetStateAction(Define::EButtonState::Release, [toRestartButton]()
		{
			toRestartButton->StopHoverPulse();
			toRestartButton->StartEffectAnimation(0.1f, 0.0f, FColor::White);
		});

	toRestartButton->SetStateAction(Define::EButtonState::Pressed, [] {
		std::wstring prevScene = SceneManager::GetPrevSceneName();
		SceneManager::ChangeScene(prevScene);
		});

	toMainButton->SetStateAction(Define::EButtonState::Hover, [toMainButton]()
		{
			toMainButton->StartHoverPulse(0.8f, 0.04f);
			toMainButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::HoverLeave, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::Release, [toMainButton]()
		{
			toMainButton->StopHoverPulse();
			toMainButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
		});

	toMainButton->SetStateAction(Define::EButtonState::Pressed, [] {
		SceneManager::ChangeScene(L"TitleScene");
		});
}

void GameOverWidgetScript::OnEnd()
{
}

void GameOverWidgetScript::OnDestroy()
{
}
