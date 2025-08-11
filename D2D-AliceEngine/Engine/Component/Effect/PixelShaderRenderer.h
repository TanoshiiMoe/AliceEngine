#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;
struct ID2D1Bitmap1;

// Direct2D ǥ�� ȿ�� ü���� �̿��� �ȼ� ����Ʈ ������
class PixelShaderRenderer : public RenderComponent
{
public:
    enum class EMode
    {
        GlitterClick,   // ���콺 �۸��� ��
        BulletImpact,   // �Ѿ� �浹 ��
        PlayerHit,      // �÷��̾� �浹
        Aura,           // ����
        Electric,       // ���� ����ũ
        Portal,         // ��Ż
        BloomPulse,     // ��� �޽�
    };

public:
    PixelShaderRenderer();
    ~PixelShaderRenderer() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetMode(EMode mode);
    void SetCenter(const FVector2& center);
    void SetSize(const FVector2& size);

private:
    void buildEffectGraph();
    void ensureWhiteBitmap();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;      // ���� ��� ����Ʈ
    Microsoft::WRL::ComPtr<ID2D1Effect> m_src;         // �Է� �ҽ� (TransformedImageSource ��)
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_whiteBmp;   // �⺻ �Է� ��Ʈ��

    EMode m_mode{ EMode::GlitterClick };
    FVector2 m_center{ 0, 0 };
    FVector2 m_size{ 640, 360 };
    float m_time{ 0.0f };
};


