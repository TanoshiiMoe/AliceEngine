#pragma once
#include "SpriteRenderer.h"

enum class EProgressBarType
{
    Linear,  // ���� �� (�����ʺ��� �߸�)
    Radial   // ���� ����� (��ä�÷� ä��)
};

class ProgressBarComponent : public SpriteRenderer
{
public:
    ProgressBarComponent() = default;
    ~ProgressBarComponent() override = default;

    void Render() override;

    // 0..1 ���൵
    void SetProgress(float p) { m_progress = (p < 0.f ? 0.f : (p > 1.f ? 1.f : p)); }
    float GetProgress() const { return m_progress; }

    void SetType(EProgressBarType type) { m_type = type; }
    EProgressBarType GetType() const { return m_type; }

    // ���� ���� ���۰� (�� ����). �⺻ -90��(���ʺ���)
    void SetStartAngleDeg(float deg) { m_startAngleDeg = deg; }
    float GetStartAngleDeg() const { return m_startAngleDeg; }

    // �ð� ���� ���� (�⺻: �ݽð� ����)
    void SetClockwise(bool cw) { m_clockwise = cw; }
    bool IsClockwise() const { return m_clockwise; }

private:
    float m_progress = 1.0f;            // 0..1
    EProgressBarType m_type = EProgressBarType::Linear;
    float m_startAngleDeg = -90.0f;     // ���� ���� ���� ��
    bool m_clockwise = false;           // �ݽð� �⺻
};
