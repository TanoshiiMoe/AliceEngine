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

	// Close
	auto closeButton = m_owner->AddComponent<ButtonComponent>();

	// Select Stage Button
	auto stage1 = m_owner->AddComponent<ButtonComponent>();
	auto stage2 = m_owner->AddComponent<ButtonComponent>();
	auto stage3 = m_owner->AddComponent<ButtonComponent>();

	// ===================================================== //
	//continueText->SetFontFromFile();
	continueText->SetText(L"이어할 곳 선택");
	continueText->SetColor(FColor(0, 234, 255, 255));
	continueText->SetFontSize(70.0f);
	continueText->SetRelativePosition(
		CoordHelper::RatioCoordToScreen(continueText->GetRelativeSize(),FVector2(-0.5,-0.5))
		+FVector2(0,-350));
	continueText->m_layer = 300;

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
	closeText->m_layer = 301;
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
	stage1->m_layer = 300;

	// ===================== stage2
	stage2->LoadData(Define::EButtonState::Idle, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Hover, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Pressed, L"UI\\MapSelectReal.png");
	stage2->LoadData(Define::EButtonState::Release, L"UI\\MapSelectReal.png");
	stage2->SetRelativePosition(FVector2(0,0));
	stage2->m_layer = 300;

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
	stage3->m_layer = 300;

	// ====================== Delegate
	stage1->SetStateAction(Define::EButtonState::Pressed, []{
		
		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(L"KangTest");
		});

	stage2->SetStateAction(Define::EButtonState::Pressed, [] {

		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(L"HiroScene");
		});

	stage3->SetStateAction(Define::EButtonState::Pressed, [] {
		
		// TODO: 여기에 스테이지 씬으로 이동하는 코드 추가
		SceneManager::ChangeScene(L"aruScene");
		});

	closeButton->SetStateAction(Define::EButtonState::Pressed, [] {
		
		SceneManager::ChangeScene(L"TitleScene");
		});
}

void SelectWidgetScript::OnEnd()
{
}

void SelectWidgetScript::OnDestroy()
{
}
