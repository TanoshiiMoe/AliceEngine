#include "CutSceneWidgetScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/ButtonComponent.h>
#include <Component/AudioComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Helpers/Logger.h>

void CutSceneWidgetScript::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(Awake);
	REGISTER_SCRIPT_METHOD(OnStart);
	REGISTER_SCRIPT_METHOD(OnEnd);
	REGISTER_SCRIPT_METHOD(OnDestroy);
}

void CutSceneWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	// 키보드 입력으로도 조작 가능
	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_SPACE))
	{
		NextImage();
	}
	else if (Input::IsKeyDown(VK_LEFT))
	{
		PrevImage();
	}
	else if (Input::IsKeyDown(VK_ESCAPE))
	{
		SkipCutScene();
	}
}

void CutSceneWidgetScript::Awake()
{
}

void CutSceneWidgetScript::OnStart()
{
	m_owner = GetOwner();
	m_owner->transform()->SetPosition(CoordHelper::RatioCoordToScreen(FVector2(0.5f, 0.5f)));

	GetCamera()->AddChildObject(m_owner);

	// 컷씬 이미지 로드
	LoadCutSceneImages();
	m_currentImageIndex = 0;

	// 컴포넌트들 생성
	m_background = m_owner->AddComponent<SpriteRenderer>();
	m_nextButton = m_owner->AddComponent<ButtonComponent>();
	m_prevButton = m_owner->AddComponent<ButtonComponent>();
	m_skipButton = m_owner->AddComponent<ButtonComponent>();

	m_nextText = m_owner->AddComponent<TextRenderComponent>();
	m_prevText = m_owner->AddComponent<TextRenderComponent>();
	m_skipText = m_owner->AddComponent<TextRenderComponent>();

	m_uiSound = m_owner->AddComponent<AudioComponent>(L"UISound");
	m_uiSound->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);

	m_background->SetDrawType(Define::EDrawType::ScreenSpace);
	m_background->SetRelativePosition(FVector2(0, 0));
	m_background->m_layer = 0;

	// Next 버튼 설정
	m_nextButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	m_nextButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	m_nextButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	m_nextButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	m_nextButton->SetRelativePosition(FVector2(400, 350));
	m_nextButton->SetRelativeScale(FVector2(0.8f, 0.8f));
	m_nextButton->m_layer = 100;

	// Prev 버튼 설정
	m_prevButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	m_prevButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	m_prevButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	m_prevButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	m_prevButton->SetRelativePosition(FVector2(-400, 350));
	m_prevButton->SetRelativeScale(FVector2(0.8f, 0.8f));
	m_prevButton->m_layer = 100;

	// Skip 버튼 설정
	m_skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	m_skipButton->SetRelativePosition(FVector2(0, 400));
	m_skipButton->SetRelativeScale(FVector2(0.8f, 0.8f));
	m_skipButton->m_layer = 100;

	// 텍스트 설정
	m_nextText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_nextText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_nextText->SetText(L"다음");
	m_nextText->SetFontSize(45.0f);
	m_nextText->SetColor(FColor::White);
	FVector2 nextTextSize = m_nextText->GetRelativeSize();
	m_nextText->SetRelativePosition(CoordHelper::RatioCoordToScreen(nextTextSize, FVector2(-0.5, -0.5)));
	m_nextText->m_layer = 101;
	m_nextText->RemoveFromParent();
	m_nextButton->AddChildComponent(m_nextText);

	m_prevText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_prevText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_prevText->SetText(L"이전");
	m_prevText->SetFontSize(45.0f);
	m_prevText->SetColor(FColor::White);
	FVector2 prevTextSize = m_prevText->GetRelativeSize();
	m_prevText->SetRelativePosition(CoordHelper::RatioCoordToScreen(prevTextSize, FVector2(-0.5, -0.5)));
	m_prevText->m_layer = 101;
	m_prevText->RemoveFromParent();
	m_prevButton->AddChildComponent(m_prevText);

	m_skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_skipText->SetText(L"건너뛰기");
	m_skipText->SetFontSize(45.0f);
	m_skipText->SetColor(FColor::White);
	FVector2 skipTextSize = m_skipText->GetRelativeSize();
	m_skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextSize, FVector2(-0.5, -0.5)));
	m_skipText->m_layer = 101;
	m_skipText->RemoveFromParent();
	m_skipButton->AddChildComponent(m_skipText);

	// 버튼 이벤트 설정
	m_nextButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
		if (m_uiSound->IsPlaying())
			m_uiSound->StopByName(L"UISound");
		m_uiSound->PlayByName(L"UISound", 0.45f);

		NextImage();
	});

	m_prevButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
		if (m_uiSound->IsPlaying())
			m_uiSound->StopByName(L"UISound");
		m_uiSound->PlayByName(L"UISound", 0.45f);

		PrevImage();
	});

	m_skipButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
		if (m_uiSound->IsPlaying())
			m_uiSound->StopByName(L"UISound");
		m_uiSound->PlayByName(L"UISound", 0.45f);

		SkipCutScene();
	});

	// 호버 효과 추가
	// Next Button 효과 (파란색 글로우)
	m_nextButton->SetStateAction(Define::EButtonState::Hover, [this]()
	{
		m_nextButton->StartHoverPulse(0.8f, 0.04f);
		m_nextButton->StartEffectAnimation(0.3f, 1.2f, FColor::Blue);
	});

	m_nextButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	{
		m_nextButton->StopHoverPulse();
		m_nextButton->StartEffectAnimation(0.2f, 0.0f, FColor::Blue);
	});

	m_nextButton->SetStateAction(Define::EButtonState::Release, [this]()
	{
		m_nextButton->StopHoverPulse();
		m_nextButton->StartEffectAnimation(0.1f, 0.0f, FColor::Blue);
	});

	// Prev Button 효과 (주황색 글로우)
	m_prevButton->SetStateAction(Define::EButtonState::Hover, [this]()
	{
		m_prevButton->StartHoverPulse(0.8f, 0.04f);
		m_prevButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	});

	m_prevButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	{
		m_prevButton->StopHoverPulse();
		m_prevButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	});

	m_prevButton->SetStateAction(Define::EButtonState::Release, [this]()
	{
		m_prevButton->StopHoverPulse();
		m_prevButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	});

	// Skip Button 효과 (빨간색 글로우)
	m_skipButton->SetStateAction(Define::EButtonState::Hover, [this]()
	{
		m_skipButton->StartHoverPulse(0.8f, 0.04f);
		m_skipButton->StartEffectAnimation(0.3f, 1.2f, FColor::Red);
	});

	m_skipButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	{
		m_skipButton->StopHoverPulse();
		m_skipButton->StartEffectAnimation(0.2f, 0.0f, FColor::Red);
	});

	m_skipButton->SetStateAction(Define::EButtonState::Release, [this]()
	{
		m_skipButton->StopHoverPulse();
		m_skipButton->StartEffectAnimation(0.1f, 0.0f, FColor::Red);
	});

	// 첫 번째 이미지 표시
	ShowImage(0);
}

void CutSceneWidgetScript::OnEnd()
{
}

void CutSceneWidgetScript::OnDestroy()
{
}

void CutSceneWidgetScript::LoadCutSceneImages()
{
	// 컷씬 이미지 경로들을 추가 (Resource/CutScene/Stage1/ 폴더의 이미지들)
	m_cutSceneImages.clear();
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\1.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\2.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\3.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\4.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\5.png");
	m_cutSceneImages.push_back(L"CutScene\\Stage1\\6.png");

	// 필요에 따라 더 많은 이미지 추가 가능
}

void CutSceneWidgetScript::ShowImage(int index)
{
	if (index < 0 || index >= m_cutSceneImages.size())
		return;

	m_currentImageIndex = index;

	// 배경 이미지 변경 - SpriteRenderer가 자동으로 화면에 맞게 조정
	m_background->LoadData(m_cutSceneImages[index]);

	// 버튼 활성화/비활성화 업데이트 (SetActive(false)가 자동으로 애니메이션 중지)
	{
		const bool hasPrev = index > 0;
		const bool hasNext = index < static_cast<int>(m_cutSceneImages.size()) - 1;

		if (!hasPrev)
		{
			m_prevButton->StopAllAnimations();
			m_prevButton->SetActive(false);
		}
		else
		{
			m_prevButton->SetActive(true);
		}

		if (!hasNext)
		{
			m_nextButton->StopAllAnimations();
			m_nextButton->SetActive(false);
		}
		else
		{
			m_nextButton->SetActive(true);
		}
	}

	OutputDebugStringW((L"Showing cutscene image: " + std::to_wstring(index + 1) + L"/" + std::to_wstring(m_cutSceneImages.size()) + L"\n").c_str());
}

void CutSceneWidgetScript::NextImage()
{
	if (m_currentImageIndex < m_cutSceneImages.size() - 1)
	{
		ShowImage(m_currentImageIndex + 1);

		// 마지막 이미지에 도달하면 Next 버튼 효과 중지
		if (m_currentImageIndex >= m_cutSceneImages.size() - 1)
		{
			m_nextButton->StopAllAnimations();
		}
	}
	else
	{
		// 마지막 이미지에서 다음 버튼을 누르면 컷씬 종료
		SkipCutScene();
	}
}

void CutSceneWidgetScript::PrevImage()
{
	if (m_currentImageIndex > 0)
	{
		ShowImage(m_currentImageIndex - 1);

		// 첫 번째 이미지에 도달하면 Prev 버튼 효과 중지
		if (m_currentImageIndex <= 0)
		{
			m_prevButton->StopAllAnimations();
		}
	}
}

void CutSceneWidgetScript::SkipCutScene()
{
	// 컷씬 종료 - 다음 씬으로 이동 (예: 게임 씬 또는 타이틀 씬)
	OutputDebugStringW(L"CutScene skipped or finished!\n");
	SceneManager::ChangeScene(L"TitleScene"); // 또는 다른 씬으로 이동
}
