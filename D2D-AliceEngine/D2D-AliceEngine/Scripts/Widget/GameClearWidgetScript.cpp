#include "GameClearWidgetScript.h"

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

void GameClearWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void GameClearWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void GameClearWidgetScript::Awake()
{
}

void GameClearWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5, 0.5)));

	GetCamera()->AddChildObject(m_owner);

	std::wstring prevScene = SceneManager::GetInstance().GetPrevSceneName();

	auto cutScene = m_owner->AddComponent<SpriteRenderer>();

	auto skipText = m_owner->AddComponent<TextRenderComponent>();
	auto skipButton = m_owner->AddComponent<ButtonComponent>();

	auto toMainText = m_owner->AddComponent<TextRenderComponent>();
	auto toMainButton = m_owner->AddComponent<ButtonComponent>();

	// ========================= //
	skipText->SetFontSize(55.0f);
	skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	skipText->SetText(L"건너뛰기");
	skipText->SetColor(FColor::White);
	skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	skipText->SetRelativeScale(FVector2(1, 1));
	skipText->m_layer = Define::Disable;
	skipText->RemoveFromParent();
	skipButton->AddChildComponent(skipText);
	
	skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	skipButton->SetRelativePosition(FVector2(0, 400));
	skipButton->SetRelativeScale(FVector2(1.f, 1.f));
	skipButton->m_layer = Define::Disable;

	toMainText->SetFontSize(55.0f);
	toMainText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	toMainText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	toMainText->SetText(L"처음 화면으로");
	toMainText->SetColor(FColor::White);
	toMainText->SetRelativePosition(CoordHelper::RatioCoordToScreen(toMainText->GetRelativeSize(), FVector2(-0.5, -0.5)));
	toMainText->SetRelativeScale(FVector2(1, 1));
	toMainText->SetRelativeRotation(0);
	toMainText->m_layer = Define::ButtonTextLayer;
	toMainText->RemoveFromParent();
	toMainButton->AddChildComponent(toMainText);

	toMainButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	toMainButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	toMainButton->SetRelativePosition(FVector2(0, 400));
	toMainButton->m_layer = Define::ButtonLayer;
	toMainButton->SetActive(false);

	skipButton->SetStateAction(Define::EButtonState::Hover, [skipButton]()
		{
			skipButton->StartHoverPulse(0.8f, 0.04f);
			skipButton->StartEffectAnimation(0.3f, 1.2f, FColor::White);
		});

	skipButton->SetStateAction(Define::EButtonState::HoverLeave, [skipButton]()
		{
			skipButton->StopHoverPulse();
			skipButton->StartEffectAnimation(0.2f, 0.0f, FColor::White);
		});

	skipButton->SetStateAction(Define::EButtonState::Release, [skipButton]()
		{
			skipButton->StopHoverPulse();
			skipButton->StartEffectAnimation(0.1f, 0.0f, FColor::White);
		});

	skipButton->SetStateAction(EButtonState::Pressed, [skipButton, skipText, cutScene,
		toMainButton
	] {
		cutScene->m_layer = Define::Disable;
		skipButton->m_layer = Define::Disable;
		skipText->m_layer = Define::Disable;
		skipButton->SetActive(false);
		toMainButton->SetActive(true);
		});

	if (prevScene == Define::Scene_Stage3)
	{
		cutScene->LoadData(L"CutScene\\Clear\\ending.png");
		cutScene->SetDrawType(EDrawType::ScreenSpace);
		const float targetW = 1920.f;
		const float targetH = 1080.f;
		const float bmpW = cutScene->GetBitmapSizeX();
		const float bmpH = cutScene->GetBitmapSizeY();
		if (bmpW > 0.f && bmpH > 0.f)
		{
			const FVector2 scale(targetW / bmpW, targetH / bmpH);
			cutScene->SetRelativeScale(scale);
			cutScene->m_layer = Define::CutSceneLayer;
		}

		skipButton->SetActive(true);
		skipButton->m_layer = Define::CutSceneLayer + Define::ButtonLayer;
		skipText->m_layer = Define::CutSceneLayer + Define::ButtonTextLayer;
	}
	else
	{
		skipButton->SetActive(false);
		toMainButton->SetActive(true);
	}

	// ====================== Delegate
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

void GameClearWidgetScript::OnEnd()
{
}

void GameClearWidgetScript::OnDestroy()
{
}
