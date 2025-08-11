#pragma once

#include <Component/RenderComponent.h>
#include <wrl/client.h>

struct ID2D1Effect;

// D2D Ŀ����(ǥ��) Vertex-like ȿ�� ü�� ���� ������
// ���� D2D�� VertexShader�� �����Ƿ�, TransformedImageSource/DisplacementMap ���� ������ �������� �ְ��� ����
class VertexShaderRenderer : public RenderComponent
{
public:
    VertexShaderRenderer();
    ~VertexShaderRenderer() override;

    void Initialize() override;
    void Release() override;
    void Update(const float& deltaSeconds) override;
    void Render() override;

    float GetBitmapSizeX() override;
    float GetBitmapSizeY() override;

    void SetSize(const FVector2& s);

private:
    void buildEffectGraph();

private:
    Microsoft::WRL::ComPtr<ID2D1Effect> m_effect;
    FVector2 m_size{ 640, 360 };
    float m_time{ 0.0f };
};


