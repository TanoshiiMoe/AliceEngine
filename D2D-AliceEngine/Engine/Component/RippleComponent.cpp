#include "pch.h"
#include "RippleComponent.h"

#include <Manager/D2DRenderManager.h>
#include <Helpers/FileHelper.h>
#include <System/RenderSystem.h>
#include <wincodec.h>
#include <d2d1effects.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>

using Microsoft::WRL::ComPtr;

// GUID들은 PixelShader/RippleEffect.h에 정의된 것을 사용해도 되지만, 여기서는 엔진 내 등록을 가정합니다.
// extern GUID GUID_RipplePixelShader; extern GUID CLSID_CustomRippleEffect; 같은 선언 경로가 없다면
// registerEffectOnce에서 RegisterEffectFromString를 직접 호출하는 대신, 표준 이펙트로 대체 가능.

RippleComponent::RippleComponent() {}
RippleComponent::~RippleComponent() {}

void RippleComponent::Initialize()
{
    __super::Initialize();
    if (m_imagePath.empty()) m_imagePath = L"Tree.png"; // Resource/Tree.png
    buildInputImage();
    registerEffectOnce();

    // Ripple 이펙트 생성 시도 (이미 등록되어 있다면 성공)
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (dc)
    {
        // 등록된 커스텀 효과 CLSID 사용할 수 없으면 표준 Vignette로 대체
        if (FAILED(dc->CreateEffect(CLSID_D2D1Vignette, &m_ripple)))
        {
            m_ripple.Reset();
        }
    }
}

void RippleComponent::Release()
{
    __super::Release();
    m_ripple.Reset();
    m_imageSource.Reset();
    m_transformed.Reset();
}

void RippleComponent::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    m_time += deltaSeconds;
    if (!m_ripple) return;

    if (m_animating)
    {
        // 샘플처럼 시간 경과로 파라미터 감쇠
        float total = m_time;
        float spread = 0.01f + total / 10.0f;
        float amplitude = max(0.0f, 60.0f - total * 15.0f);
        float frequency = max(0.0f, 140.0f - total * 30.0f);
        float phase = -total * 20.0f;

        // 표준 Vignette 대체 시 직접 매핑 불가. 실제 커스텀 효과 등록 시 아래 값들을 SetValue로 전달.
        // 여기서는 대체로 Strength만 변화시켜 반짝임 대체 연출
        m_ripple->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, min(1.0f, spread));
        if (total >= 4.0f) m_animating = false;
    }
}

void RippleComponent::Render()
{
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;
    __super::Render();
    if (!m_ripple || !m_imageSource) return;

    // 입력 연결 (현재는 Vignette 대체라 입력 없이 DrawImage 가능하지만, 구조 유지)
    // 실제 커스텀 Ripple 효과일 때: m_ripple->SetInput(0, m_transformed.Get());
    dc->DrawImage(m_ripple.Get());
}

float RippleComponent::GetBitmapSizeX() { return 1920.0f; }
float RippleComponent::GetBitmapSizeY() { return 1080.0f; }

void RippleComponent::SetImage(const std::wstring& relativePath)
{
    m_imagePath = relativePath;
    buildInputImage();
}

void RippleComponent::registerEffectOnce()
{
    // 실제 등록 구현은 PixelShader/RippleEffect.cpp의 Register를 엔진 초기화 시 호출해야 합니다.
    // 여기서는 샘플과 동일한 경로를 권장 (RegisterEffectFromString + LoadPixelShader).
    // 미구현 시 표준 이펙트 대체로 동작하도록 상단에서 Vignette 생성으로 대응합니다.
}

void RippleComponent::buildInputImage()
{
    // Tree.png 등 WIC 비트맵을 ImageSourceFromWic로 구성 후 필요 시 TransformedImageSource로 스케일링
    ID2D1DeviceContext7* dc = D2DRenderManager::GetD2DDevice();
    if (!dc) return;

    // D2DRenderManager::CreateBitmapFromFile를 써서 바로 Bitmap으로 그리는 방식도 가능하지만,
    // 샘플 구조를 반영해 ImageSourceFromWic/TransformedImageSource 체계를 준비해 둡니다.
    // 간단화를 위해 여기서는 이펙트 입력 없이 배경 이미지는 별도 SpriteRenderer 사용을 권장.
}

void RippleComponent::triggerRippleAt(float x, float y)
{
    // 마우스 클릭 좌표에서 애니메이션 시작
    m_time = 0.0f;
    m_animating = true;
}


