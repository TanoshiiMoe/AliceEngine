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
#include <GameManager/GamePlayManager.h>

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
    if (!m_isTransitioning && m_canAdvance && (Input::IsKeyDown(VK_RETURN)))
	{
		NextImage();
	}
    //else if (!m_isTransitioning && Input::IsKeyDown(VK_LEFT))
    //{
    //    PrevImage();
    //}
    else if (!m_isTransitioning && Input::IsKeyDown(VK_ESCAPE))
	{
		SkipCutScene();
	}

    // 진행 중인 전환 스텝 처리
    if (m_isTransitioning)
    {
        StepTransition(TimerManager::GetInstance().unscaledDeltaTime);
    }
}

void CutSceneWidgetScript::Awake()
{
    m_isAlive = true;
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
    // 월드 가림용 백드롭을 가장 먼저 생성하여 항상 뒤를 완전 차단
    EnsureBackdropOverlay();

    m_background = m_owner->AddComponent<SpriteRenderer>();
    //m_nextButton = m_owner->AddComponent<ButtonComponent>();
    //m_prevButton = m_owner->AddComponent<ButtonComponent>();
	m_skipButton = m_owner->AddComponent<ButtonComponent>();

    //m_nextText = m_owner->AddComponent<TextRenderComponent>();
    //m_prevText = m_owner->AddComponent<TextRenderComponent>();
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
    // 임시로 layer만 설정 (상대 레이어 적용)
    m_background->m_layer = 0 + m_relativeLayer;

	// Next 버튼 설정
    // Next 버튼 주석 처리

	// Prev 버튼 설정
    // Prev 버튼 주석 처리

	// Skip 버튼 설정
	m_skipButton->LoadData(Define::EButtonState::Idle, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Hover, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Pressed, L"UI\\Button_Idle.png");
	m_skipButton->LoadData(Define::EButtonState::Release, L"UI\\Button_Idle.png");
	m_skipButton->SetRelativePosition(FVector2(0, 400));
	m_skipButton->SetRelativeScale(FVector2(0.8f, 0.8f));
    m_skipButton->m_layer = 100 + m_relativeLayer;

	// 텍스트 설정
    // Next 텍스트 주석 처리

    // Prev 텍스트 주석 처리

	m_skipText->SetFontSize(45.0f);
	m_skipText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
	m_skipText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
	m_skipText->SetText(L"건너뛰기");
	m_skipText->SetColor(FColor::White);
	FVector2 skipTextSize = m_skipText->GetRelativeSize();
	m_skipText->SetRelativePosition(CoordHelper::RatioCoordToScreen(skipTextSize, FVector2(-0.5, -0.5)));
    m_skipText->m_layer = 101 + m_relativeLayer;
	m_skipText->RemoveFromParent();
	m_skipButton->AddChildComponent(m_skipText);

    // 안내 문구 생성 (초기 비가시, 스킵 버튼 오른쪽에 배치)
    m_guideText = m_owner->AddComponent<TextRenderComponent>();
    m_guideText->SetFontSize(40.0f);
    m_guideText->SetFontFromFile(L"Fonts\\April16thTTF-Promise.ttf");
    m_guideText->SetFont(L"사월십육일 TTF 약속", L"ko-KR");
    m_guideText->SetText(L"엔터를 눌러 다음으로 넘어가세요");
    m_guideText->SetColor(FColor::White);
    FVector2 guideSize = m_guideText->GetRelativeSize();
    // 스킵 버튼 오른쪽(오프셋 220,0)
    m_guideText->SetRelativePosition(FVector2(340, -20));
    m_guideText->m_layer = 101 + m_relativeLayer;
    m_guideText->RemoveFromParent();
    m_skipButton->AddChildComponent(m_guideText);
    // 시작 시 감춤
    m_guideAlpha = 0.0f;
    m_guideText->SetOpacity(0.0f);
    m_guideFadeElapsed = 0.0f;

	// 버튼 이벤트 설정
    // Next 버튼 핸들러 주석 처리

    // Prev 버튼 핸들러 주석 처리

	m_skipButton->SetStateAction(Define::EButtonState::Pressed, [this]()
	{
		m_uiSound->StopByName(L"UISound");
		m_uiSound->PlayByName1(L"UISound", 0.45f);
        GamePlayManager::GetInstance().StartGame();

		SkipCutScene();
	});

	// 호버 효과 추가
	// Next Button 효과 (파란색 글로우)
	//m_nextButton->SetStateAction(Define::EButtonState::Hover, [this]()
	//{
	//	m_nextButton->StartHoverPulse(0.8f, 0.04f);
	//	m_nextButton->StartEffectAnimation(0.3f, 1.2f, FColor::Blue);
	//});
    //
	//m_nextButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	//{
	//	m_nextButton->StopHoverPulse();
	//	m_nextButton->StartEffectAnimation(0.2f, 0.0f, FColor::Blue);
	//});
    //
	//m_nextButton->SetStateAction(Define::EButtonState::Release, [this]()
	//{
	//	m_nextButton->StopHoverPulse();
	//	m_nextButton->StartEffectAnimation(0.1f, 0.0f, FColor::Blue);
	//});
    //
	//// Prev Button 효과 (주황색 글로우)
	//m_prevButton->SetStateAction(Define::EButtonState::Hover, [this]()
	//{
	//	m_prevButton->StartHoverPulse(0.8f, 0.04f);
	//	m_prevButton->StartEffectAnimation(0.3f, 1.2f, FColor::Orange);
	//});
    //
	//m_prevButton->SetStateAction(Define::EButtonState::HoverLeave, [this]()
	//{
	//	m_prevButton->StopHoverPulse();
	//	m_prevButton->StartEffectAnimation(0.2f, 0.0f, FColor::Orange);
	//});
    //
	//m_prevButton->SetStateAction(Define::EButtonState::Release, [this]()
	//{
	//	m_prevButton->StopHoverPulse();
	//	m_prevButton->StartEffectAnimation(0.1f, 0.0f, FColor::Orange);
	//});

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

    // 각 컷마다 2초 지연 후 가이드 표시
    ShowGuideAfterDelay();
}

void CutSceneWidgetScript::OnEnd()
{
    m_isAlive = false;
    StopGuideTimers();
}

void CutSceneWidgetScript::OnDestroy()
{
    m_isAlive = false;
    StopGuideTimers();
    m_guideText = nullptr;
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
            spr->m_layer = desc.layer + m_relativeLayer;
            m_activeOverlays.push_back(spr);
        }
    }

    // 버튼 활성화/텍스트 업데이트 (Next/Prev는 현재 주석 처리)
    {
        const bool hasPrev = index > 0;
        const bool isLast = index >= static_cast<int>(m_cutSceneImages.size()) - 1;

        // 진행 가능 여부 초기화 및 가이드 리셋
        m_canAdvance = false;
        StopGuideTimers();
        if (m_guideText)
        {
            m_guideAlpha = 0.0f;
            m_guideText->SetOpacity(0.0f);
        }
        // 2초 뒤 가이드 출력
        ShowGuideAfterDelay();
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
        GamePlayManager::GetInstance().StartGame();
        TimerManager::GetInstance().SetGlobalTimeScale(1.0f);
        // 리소스 정리 및 컴포넌트 파괴
        m_isTransitioning = false;
        ClearOpacityOnAll();
        ClearOverlays();
        if (m_fadeOverlay)
        {
            m_fadeOverlay->RemoveFromParent();
            m_fadeOverlay->Release();
            m_fadeOverlay = nullptr;
        }
        if (m_backdropOverlay)
        {
            m_backdropOverlay->RemoveFromParent();
            m_backdropOverlay->Release();
            m_backdropOverlay = nullptr;
        }
        if (m_background)
        {
            m_background->RemoveFromParent();
            m_background->Release();
            m_background = nullptr;
        }
        GetWorld()->RemoveObject(owner.Get());
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
    // 외부에서 지정한 다음 씬으로 이동
    //SceneManager::ChangeScene(m_nextSceneName.c_str());
    // 글로벌 타임스케일 복구
    TimerManager::GetInstance().SetGlobalTimeScale(1.0f);

    // 리소스 정리 및 컴포넌트 파괴
    m_isTransitioning = false;
    ClearOpacityOnAll();
    ClearOverlays();
    if (m_fadeOverlay)
    {
        m_fadeOverlay->RemoveFromParent();
        m_fadeOverlay->Release();
        m_fadeOverlay = nullptr;
    }
    if (m_backdropOverlay)
    {
        m_backdropOverlay->RemoveFromParent();
        m_backdropOverlay->Release();
        m_backdropOverlay = nullptr;
    }
    if (m_background)
    {
        m_background->RemoveFromParent();
        m_background->Release();
        m_background = nullptr;
    }
    // 자신 제거
    GetWorld()->RemoveObject(owner.Get());
}

void CutSceneWidgetScript::ShowGuideAfterDelay()
{
    TimerManager::GetInstance().ClearTimer(m_guideDelayTimer);
    TimerManager::GetInstance().SetTimer(
        m_guideDelayTimer,
        [this]() {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideDelayTimer); return; }
            // 2초 경과 후 가이드 페이드 인 시작 및 입력 허용
            m_canAdvance = true;
            StartGuideFadeIn(0.3f);
        },
        2.0f,
        false,
        2.0f
    );
}

void CutSceneWidgetScript::StartGuideFadeIn(float durationSec)
{
    TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
    m_guideAlpha = 0.0f;
    m_guideFadeElapsed = 0.0f;
    TimerManager::GetInstance().SetTimer(
        m_guideFadeTimer,
        [this, durationSec]() mutable {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideFadeTimer); return; }
            const float dt = 1.0f / 60.0f;
            m_guideFadeElapsed += dt;
            float t = m_guideFadeElapsed / durationSec;
            if (t > 1.0f) t = 1.0f;
            m_guideAlpha = t;
            SetGuideAlpha(m_guideAlpha);
            if (t >= 1.0f)
            {
                TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
                // 페이드 인 완료 후 반짝임 시작
            }
        },
        0.0f,
        true,
        0.0f
    );
}

void CutSceneWidgetScript::StartGuideBlink(float intervalSec)
{
    TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer);
    // 0.5초마다 토글
    TimerManager::GetInstance().SetTimer(
        m_guideBlinkTimer,
        [this, visible = true]() mutable {
            if (!m_isAlive) { TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer); return; }
            visible = !visible;
            SetGuideAlpha(visible ? 1.0f : 0.3f);
        },
        intervalSec,
        true,
        intervalSec
    );
}

void CutSceneWidgetScript::StopGuideTimers()
{
    TimerManager::GetInstance().ClearTimer(m_guideDelayTimer);
    TimerManager::GetInstance().ClearTimer(m_guideFadeTimer);
    TimerManager::GetInstance().ClearTimer(m_guideBlinkTimer);
}

void CutSceneWidgetScript::SetGuideAlpha(float a01)
{
    if (!m_guideText) return;
    if (a01 < 0.f) a01 = 0.f; if (a01 > 1.f) a01 = 1.f;
    m_guideText->SetOpacity(a01);
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

    // 초기 상태
    ApplyOpacityToAll(1.f);
    EnsureFadeOverlay();
    if (m_fadeOverlay) { m_fadeOverlay->SetOpacity(0.f); m_fadeOverlay->m_layer = 9999 + m_relativeLayer; }
}

void CutSceneWidgetScript::StepTransition(float dt)
{
    // 컷씬 진행 중에는 타임스케일 0이더라도 자체 dt로 동작해야 하므로 전달된 dt만 사용
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
    // 페이드 오버레이는 컷씬 요소들보다 위 (상대 레이어 적용)
    m_fadeOverlay->m_layer = 9999 + m_relativeLayer;
}

void CutSceneWidgetScript::EnsureBackdropOverlay()
{
    if (m_backdropOverlay) return;
    // 전체 화면을 덮는 검은 1x1 텍스처를 가장 뒤 레이어에 깔아 월드를 완전히 차단
    m_backdropOverlay = m_owner->AddComponent<SpriteRenderer>();
    m_backdropOverlay->SetDrawType(Define::EDrawType::ScreenSpace);
    m_backdropOverlay->LoadData(L"CutScene\\White.png");
    const float bw = m_backdropOverlay->GetBitmapSizeX();
    const float bh = m_backdropOverlay->GetBitmapSizeY();
    if (bw > 0 && bh > 0)
    {
        m_backdropOverlay->SetRelativeScale(FVector2(Define::SCREEN_WIDTH / bw, Define::SCREEN_HEIGHT / bh));
    }
    m_backdropOverlay->SetRelativePosition(FVector2(0, 0));
    // 컷씬 전체보다 뒤에 위치 (상대 레이어 적용)
    m_backdropOverlay->m_layer = -10000 + m_relativeLayer;
}
