#include "pch.h"
#include "ProgressBarComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Component/RenderComponent.h>

void ProgressBarComponent::Render()
{
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (!context || !m_bitmap) return;
    // 부모(SpriteRenderer)의 Render는 비트맵 전체를 그리므로 호출하지 않음.
    // 대신 상위(RenderComponent)의 변환/상태 설정만 수행
    RenderComponent::Render();

    // 원본 스프라이트 소스 영역
    float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
    float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
    float srcL = slice.srcX;
    float srcT = slice.srcY;

    if (m_type == EProgressBarType::Linear)
    {
        // 진행 비율만큼 오른쪽에서 잘라내기
        float visibleW = cropW * m_progress;     // 소스 픽셀 기준 가시 폭
        if (visibleW <= 0.0f) return; // 전부 숨김

        // 소스 사각형: 왼쪽부터 visibleW 만큼만 보이기
        D2D1_RECT_F srcRect = {
            srcL,
            srcT,
            srcL + visibleW,
            srcT + cropH
        };

        // 화면 대상: 컴포넌트 스케일을 반영한 크기 계산
        // 전체 비트맵의 화면 폭/높이
        const float fullDestWForBitmap = SpriteRenderer::GetRelativeSize().x; // Sx * bmpW
        const float fullDestHForBitmap = SpriteRenderer::GetRelativeSize().y; // Sy * bmpH
        // 현재 crop 영역(원본의 일부분)에 해당하는 화면 폭/높이
        const float destFullWForCrop = fullDestWForBitmap * (cropW / (float)SpriteRenderer::GetBitmapSizeX());
        const float destFullHForCrop = fullDestHForBitmap * (cropH / (float)SpriteRenderer::GetBitmapSizeY());
        // 진행도에 따른 가시 폭(화면 단위)
        const float destVisibleW = destFullWForCrop * (visibleW / cropW); // == destFullWForCrop * m_progress

        // 피벗을 고려한 좌표 (좌측 고정 효과: 좌측은 변하지 않고 우측만 줄어듦)
        const float destLeft = -destFullWForCrop * spriteInfo.pivotX;
        const float destTop  = -destFullHForCrop * spriteInfo.pivotY;

        D2D1_RECT_F destRect = {
            destLeft,
            destTop,
            destLeft + destVisibleW,
            destTop + destFullHForCrop
        };

        context->DrawBitmap(m_bitmap.get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
        return;
    }
    else // Radial
    {
        // 간단한 원형 마스크: 진행 각도만큼 부채꼴로 클리핑 (고정 파이프라인 내에서 간이 구현)
        // 주의: 고급 마스킹은 별도 효과/레이어 필요. 여기서는 간단히 DrawImage 효과 기반의 Clip은 생략하고,
        // 진행율을 8분할 근사로 표현 (필요 시 확장)

        // 일단 Linear와 동일하게 전체 그리기 (기본 버전)
        // TODO: 향후 ID2D1CommandList 또는 Geometry로 파이 마스크 적용 확대 가능
        D2D1_RECT_F srcRect = {
            srcL,
            srcT,
            srcL + cropW,
            srcT + cropH
        };
        const float offsetX = -cropW * spriteInfo.pivotX;
        const float offsetY = -cropH * spriteInfo.pivotY;
        D2D1_RECT_F destRect = {
            offsetX,
            offsetY,
            offsetX + cropW,
            offsetY + cropH
        };
        D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
        return;
    }
}
