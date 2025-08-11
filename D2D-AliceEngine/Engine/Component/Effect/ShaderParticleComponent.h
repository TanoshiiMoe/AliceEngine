#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;

// D2D Ŀ���� PixelShader�� ����ϴ� ��ƼŬ/����Ʈ ������Ʈ
// - D2DRenderManager�� ��Ÿ�ӿ� HLSL�� �������ϰ� LoadPixelShader�� �ε��մϴ�.
// - �� ������Ʈ�� DrawPixelShader ����Ʈ�� �����Ͽ� �簢�� ������ �׸��ϴ�.
class ShaderParticleComponent : public RenderComponent
{
public:
    enum class EShaderKind
    {
        Blending,
        ColorChange,
        Distortion,
        Special,
        Environment,
        Physics,
    };

public:
    ShaderParticleComponent();
    ~ShaderParticleComponent() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetShader(EShaderKind kind);
    void SetSize(const FVector2& size);     // ȭ��/���忡�� �׸� �ڽ� ũ��
    void SetAdditive(bool additive);

private:
    void ensureEffect();
    void updateConstantBuffer();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;
    EShaderKind m_kind = EShaderKind::Special;
    FVector2 m_size = FVector2(256.0f, 256.0f);
    bool m_additive = true;
    float m_time = 0.0f;
};

