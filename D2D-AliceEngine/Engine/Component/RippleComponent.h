#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;
struct ID2D1ImageSourceFromWic;
struct ID2D1TransformedImageSource;

// D2D Custom Pixel Shader (Ripple) ����Ʈ ������
// PixelShader/RippleEffect.* ������ ������ ���� ����
class RippleComponent : public RenderComponent
{
public:
    RippleComponent();
    ~RippleComponent() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetImage(const std::wstring& relativePath); // �⺻: Resource/Tree.png

private:
    void registerEffectOnce();
    void buildInputImage();
    void triggerRippleAt(float x, float y);

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_ripple;
    Microsoft::WRL::ComPtr<ID2D1ImageSourceFromWic> m_imageSource;
    Microsoft::WRL::ComPtr<ID2D1TransformedImageSource> m_transformed;

    std::wstring m_imagePath; // Resource ��� ���
    float m_time{ 0.0f };
    bool m_animating{ false };
};


