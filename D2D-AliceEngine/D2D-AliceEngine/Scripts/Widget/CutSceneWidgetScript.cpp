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
    if (!m_isTransitioning && (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_SPACE)))
	{
		NextImage();
	}
    else if (!m_isTransitioning && Input::IsKeyDown(VK_LEFT))
	{
		PrevImage();
	}
    else if (!m_isTransitioning && Input::IsKeyDown(VK_ESCAPE))
	{
		SkipCutScene();
	}

    // 진행 중인 전환 스텝 처리
    if (m_isTransitioning)
    {
        StepTransition(deltaSeconds);
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
    // 배경 사이즈를 원하는 값으로 지정 (예: 1920x1080)
    // SpriteRenderer 엔진 전역 변경 없이, 여기서 스케일을 직접 계산해 적용
    // 원본 비트맵 크기 대비 목표 픽셀 크기로 스케일을 정합니다.
    const float targetW = 1920.f;
    const float targetH = 1080.f;
    // 아직 비트맵이 로드되지 않았으므로, 첫 ShowImage에서 실제 스케일을 적용합니다.
    // 임시로 layer만 설정
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

    // 배경 이미지 변경하고 원하는 픽셀 사이즈(1920x1080)로 스케일 적용
    m_background->LoadData(m_cutSceneImages[index]);
    {
        const float targetW = 1920.f;
        const float targetH = 1080.f;
        const float bmpW = m_background->GetBitmapSizeX();
        const float bmpH = m_background->GetBitmapSizeY();
        if (bmpW > 0.f && bmpH > 0.f)
        {
            const FVector2 scale(targetW / bmpW, targetH / bmpH);
            m_background->SetRelativeScale(scale);
        }
    }

    // 이전 오버레이 제거
    ClearOverlays();
    // 해당 페이지에 등록된 추가 스프라이트 생성
    auto it = m_pageOverlays.find(index);
    if (it != m_pageOverlays.end())
    {
        for (const auto& desc : it->second)
        {
            auto* spr = m_owner->AddComponent<SpriteRenderer>();
            spr->SetDrawType(Define::EDrawType::ScreenSpace);
            spr->LoadData(desc.path);
            // 사이즈 지정이 있으면 스케일로 적용
            const float w = spr->GetBitmapSizeX();
            const float h = spr->GetBitmapSizeY();
            if (w > 0.f && h > 0.f && desc.size.x > 0.f && desc.size.y > 0.f)
            {
                spr->SetRelativeScale(FVector2(desc.size.x / w, desc.size.y / h));
            }
            spr->SetRelativePosition(desc.position);
            spr->m_layer = desc.layer;
            m_activeOverlays.push_back(spr);
        }
    }

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
    if (m_isTransitioning) return;
    if (m_currentImageIndex < m_cutSceneImages.size() - 1)
    {
        StartTransitionTo(m_currentImageIndex + 1, 0.8f);
    }
	else
	{
		// 마지막 이미지에서 다음 버튼을 누르면 컷씬 종료
		SkipCutScene();
	}
}

void CutSceneWidgetScript::PrevImage()
{
    if (m_isTransitioning) return;
    if (m_currentImageIndex > 0)
    {
        StartTransitionTo(m_currentImageIndex - 1, 0.8f);
    }
}

void CutSceneWidgetScript::SkipCutScene()
{
	// 컷씬 종료 - 다음 씬으로 이동 (예: 게임 씬 또는 타이틀 씬)
	OutputDebugStringW(L"CutScene skipped or finished!\n");
	SceneManager::ChangeScene(L"TitleScene"); // 또는 다른 씬으로 이동
}

// 모든 관련 스프라이트에 동일 Opacity 적용 (배경 + 오버레이)
void CutSceneWidgetScript::ApplyOpacityToAll(float alpha)
{
    if (m_background) m_background->SetOpacity(alpha);
    for (auto* spr : m_activeOverlays)
        if (spr) spr->SetOpacity(alpha);
}

void CutSceneWidgetScript::ClearOpacityOnAll()
{
    if (m_background) m_background->ClearEffect();
    for (auto* spr : m_activeOverlays)
        if (spr) spr->ClearEffect();
}

// 페이드 아웃하고 나서 페이지 갱신 그 이후 페이드 인
void CutSceneWidgetScript::StartTransitionTo(int targetIndex, float durationSec)
{
    if (targetIndex < 0 || targetIndex >= (int)m_cutSceneImages.size()) return;
    m_isTransitioning = true;
    m_transitionHalf = durationSec * 0.5f;
    m_transitionElapsed = 0.f;
    m_transitionSwitchover = false;
    m_targetIndex = targetIndex;

    // 초기 상태: 완전 가시 상태
    ApplyOpacityToAll(1.f);
    EnsureFadeOverlay();
    if (m_fadeOverlay) { m_fadeOverlay->SetOpacity(0.f); m_fadeOverlay->m_layer = 9999; }
}

void CutSceneWidgetScript::StepTransition(float dt)
{
    m_transitionElapsed += dt;
    float t = Math::Clamp(m_transitionElapsed / m_transitionHalf, 0.f, 1.f);
    float eased = t * t * (3.f - 2.f * t);

    if (!m_transitionSwitchover)
    {
        // 1 - 0 페이드 아웃
        ApplyOpacityToAll(1.f - eased);
        if (m_fadeOverlay) m_fadeOverlay->SetOpacity(eased * 0.4f);
        if (t >= 1.f)
        {
            ShowImage(m_targetIndex);
            m_transitionElapsed = 0.f;
            m_transitionSwitchover = true;
        }
    }
    else
    {
        // 0 - 1 페이드 인
        ApplyOpacityToAll(eased);
        if (m_fadeOverlay) m_fadeOverlay->SetOpacity((1.f - eased) * 0.4f);
        if (t >= 1.f)
        {
            ClearOpacityOnAll();
            if (m_fadeOverlay) { m_fadeOverlay->SetOpacity(0.f); m_fadeOverlay->m_layer = -1000; }
            m_isTransitioning = false;
            m_transitionElapsed = 0.f;
            m_transitionSwitchover = false;
            m_targetIndex = -1;
        }
    }
}

// 오버레이 관리 유틸
void CutSceneWidgetScript::ClearOverlays()
{
    for (auto* spr : m_activeOverlays)
    {
        if (spr)
        {
            spr->RemoveFromParent();
            spr->Release();
        }
    }
    m_activeOverlays.clear();
}

void CutSceneWidgetScript::AddSpriteToPage(int pageIndex, const CutSpriteDesc& desc)
{
    m_pageOverlays[pageIndex].push_back(desc);
}

void CutSceneWidgetScript::EnsureFadeOverlay()
{
    if (m_fadeOverlay) return;
    // 전체 화면을 덮는 검은 1x1 텍스처 필요: CutScene\\Black.png
    m_fadeOverlay = m_owner->AddComponent<SpriteRenderer>();
    m_fadeOverlay->SetDrawType(Define::EDrawType::ScreenSpace);
    m_fadeOverlay->LoadData(L"CutScene\\Black.png");
    const float w = m_fadeOverlay->GetBitmapSizeX();
    const float h = m_fadeOverlay->GetBitmapSizeY();
    if (w > 0 && h > 0)
    {
        m_fadeOverlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / w, Define::SCREEN_HEIGHT / h));
    }
    m_fadeOverlay->SetRelativePosition(FVector2(0, 0));
    m_fadeOverlay->m_layer = 9999;
}
