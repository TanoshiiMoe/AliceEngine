#include "SelectWidgetScript.h"
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

void SelectWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void SelectWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void SelectWidgetScript::Awake()
{
}

void SelectWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	auto background = m_owner->AddComponent<SpriteRenderer>();

	auto continueText = m_owner->AddComponent<TextRenderComponent>();
	auto closeText = m_owner->AddComponent<TextRenderComponent>();

	//auto uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	//uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::UI);

	// Close
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	// Select Stage Button
	auto stage1 = m_owner->AddComponent<ButtonComponent>();
	auto stage2 = m_owner->AddComponent<ButtonComponent>();
	auto stage3 = m_owner->AddComponent<ButtonComponent>();

	// ===================================================== //
	background->LoadData(L"");
	
	
	
	
	
	
	
	
	continueText->SetFontSize(70.0f);
	continueText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	continueText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	continueText->SetText(L"임무 선택");
	continueText->SetColor(FColor(0, 234, 255, 255));
	continueText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueText->GetRelativeSize(),FVector2(-0.5,-0.5))
		+FVector2(0,-350));
	continueText->m_layer = Define::NormalTextLayer;

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
	closeText->m_layer = Define::NormalTextLayer;
	closeText->RemoveFromParent();
	closeButton->AddChildComponent(closeText);

	// ===================== closeButton
	closeButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	closeButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	closeButton->SetRelativePosition(FVector2(0, 350));

	// ===================== stage1
	stage1->LoadData(Define::EButtonState::Idle, L"UI\\MapSelectExam.png");
	stage1->LoadData(Define::EButtonState::Hover, L"UI\\MapSelectExam.png");
	stage1->LoadData(Define::EButtonState::Pressed, L"UI\\MapSelectExam.png");
	stage1->LoadData(Define::EButtonState::Release, L"UI\\MapSelectExam.png");
	FVector2 stage1Size = stage1->GetRelativeSize();
	stage1->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage1Size, FVector2(0 ,0))
		+ FVector2(-500, 0)
	);
	stage1->SetRelativeScale(FVector2(1, 1));
	stage1->m_layer = Define::NormalTextLayer;

	// ===================== stage2
	stage2->LoadData(Define::EButtonState::Idle, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Hover, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Pressed, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Release, L"UI\\MapSelectReal.png");
	stage2->SetRelativePosition(FVector2(0,0));
	stage2->m_layer = Define::NormalTextLayer;

	// ===================== stage3
	stage3->LoadData(Define::EButtonState::Idle, L"UI\\MapSelectBoss.png");
	stage3->LoadData(Define::EButtonState::Hover, L"UI\\MapSelectBoss.png");
	stage3->LoadData(Define::EButtonState::Pressed, L"UI\\MapSelectBoss.png");
	stage3->LoadData(Define::EButtonState::Release, L"UI\\MapSelectBoss.png");
	FVector2 stage3Size = stage3->GetRelativeSize();
	stage3->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(stage3Size, FVector2(0, 0))
		+ FVector2(500, 0)
	);
	stage3->SetRelativeScale(FVector2(1, 1));
	stage3->m_layer = Define::NormalTextLayer;

	// ====================== Delegate & Hover Effects
	
	// stage1 Button 효과 (초록색 글로우 - 쉬운 스테이지)
	stage1->SetStateAction(Define::EButtonState::Hover, [stage1]()
	{
		stage1->StartHoverPulse(0.8f, 0.04f);
		stage1->StartEffectAnimation(0.3f, 1.2f, FColor::Green);
	});

	stage1->SetStateAction(Define::EButtonState::HoverLeave, [stage1]()
	{
		stage1->StopHoverPulse();
		stage1->StartEffectAnimation(0.2f, 0.0f, FColor::Green);
	});

	stage1->SetStateAction(Define::EButtonState::Release, [stage1]()
	{
		stage1->StopHoverPulse();
		stage1->StartEffectAnimation(0.1f, 0.0f, FColor::Green);
	});

	stage1->SetStateAction(Define::EButtonState::Pressed, []{
		
		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(Define::Scene_Stage1);
		});

	// stage2 Button 효과 (노란색 글로우 - 보통 스테이지)
	stage2->SetStateAction(Define::EButtonState::Hover, [stage2]()
	{
		stage2->StartHoverPulse(0.8f, 0.04f);
		stage2->StartEffectAnimation(0.3f, 1.2f, FColor::Yellow);
	});

	stage2->SetStateAction(Define::EButtonState::HoverLeave, [stage2]()
	{
		stage2->StopHoverPulse();
		stage2->StartEffectAnimation(0.2f, 0.0f, FColor::Yellow);
	});

	stage2->SetStateAction(Define::EButtonState::Release, [stage2]()
	{
		stage2->StopHoverPulse();
		stage2->StartEffectAnimation(0.1f, 0.0f, FColor::Yellow);
	});

	stage2->SetStateAction(Define::EButtonState::Pressed, [] {

		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(Define::Scene_Stage2);
		});

	// stage3 Button 효과 (빨간색 글로우 - 어려운 스테이지/보스)
	stage3->SetStateAction(Define::EButtonState::Hover, [stage3]()
	{
		stage3->StartHoverPulse(0.8f, 0.04f);
		stage3->StartEffectAnimation(0.3f, 1.2f, FColor::Red);
	});

	stage3->SetStateAction(Define::EButtonState::HoverLeave, [stage3]()
	{
		stage3->StopHoverPulse();
		stage3->StartEffectAnimation(0.2f, 0.0f, FColor::Red);
	});

	stage3->SetStateAction(Define::EButtonState::Release, [stage3]()
	{
		stage3->StopHoverPulse();
		stage3->StartEffectAnimation(0.1f, 0.0f, FColor::Red);
	});

	stage3->SetStateAction(Define::EButtonState::Pressed, [] {
		
		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(Define::Scene_Stage3);
		});

	// closeButton 효과 (주황색 글로우)
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

	closeButton->SetStateAction(Define::EButtonState::Pressed,[] {
		
		SceneManager::ChangeScene(L"TitleScene");
		});
}

void SelectWidgetScript::OnEnd()
{
}

void SelectWidgetScript::OnDestroy()
{
}
