/*
* @brief : ������ ����� ���ƿ�(����) ���� ��ũ��Ʈ�Դϴ�.
* @details :
*   - ������Ʈ�� �����ϸ� ���ο��� `SpriteRenderer`�� �����Ͽ� ScreenSpace �������� ȭ�� ��ü�� �����ϴ�.
*   - Direct2D ����Ʈ(Vignette + Spot/Point Lighting + Opacity + ���� CrossFade)�� ������
*     �����ڸ����� �߾����� ��ο����� ���������� ���� �������� ��ȯ�Ǵ� ������ �����մϴ�.
*   - `SetDuration(seconds)`�� �� ���� �ð���, `SetMaxEdgeAlpha(a01)`�� ���� ������ ������ �����մϴ�.
*   - `SetMode()`�� ������ Ÿ��(SpotDiffuse/PointDiffuse/SpotSpecular/VignetteOnly)�� ������ �� �ֽ��ϴ�.
*/
#pragma once
#include <Component/ScriptComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;
struct ID2D1Bitmap1;
class SpriteRenderer;

enum class EBlackOutLightingMode
{
    VignetteOnly,
    PointDiffuse,
    SpotDiffuse,
    SpotSpecular,
};

class BlackOutWidgetScript : public ScriptComponent
{
public:
    void Initialize() override;
    void OnStart() override;
    void Update(const float& deltaSeconds) override;

    void SetDuration(float seconds) { m_duration = (seconds <= 0.f ? 0.01f : seconds); }
    void SetMaxEdgeAlpha(float a01) { m_maxEdgeAlpha = (a01 < 0.f ? 0.f : (a01 > 1.f ? 1.f : a01)); }
    void SetMode(EBlackOutLightingMode mode) { m_mode = mode; }
    void UseCrossFade(bool use) { m_useCrossFade = use; }

private:
    // ���� ����
    bool  m_active = true;
    float m_elapsed = 0.f;
    float m_duration = 1.6f;
    float m_maxEdgeAlpha = 1.0f;

    // ���� ��������
    SpriteRenderer* m_overlay = nullptr;
    int m_overlayLayer = 999999;

    // ����Ʈ��
    Microsoft::WRL::ComPtr<ID2D1Effect> m_vignetteEffect;
    Microsoft::WRL::ComPtr<ID2D1Effect> m_lightEffect;   // Spot/Point/Specular
    Microsoft::WRL::ComPtr<ID2D1Effect> m_opacityEffect; // ���� ����
    Microsoft::WRL::ComPtr<ID2D1Effect> m_crossFadeEffect; // ����: ������ ���� ��ȯ
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_blackBitmap;     // CrossFade��

    // �ɼ�
    EBlackOutLightingMode m_mode = EBlackOutLightingMode::SpotDiffuse;
    bool m_useCrossFade = true;
};


