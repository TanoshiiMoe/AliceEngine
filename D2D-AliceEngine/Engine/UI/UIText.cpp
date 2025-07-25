#include "pch.h"
#include "UIText.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Component/TransformComponent.h>
#include <System/RenderSystem.h>
#include <Math/TColor.h>

UIText::UIText()
{
	m_color = FColor::Black;
	drawType = EDrawType::ScreenSpace;
}

UIText::UIText(const std::wstring& content, const FColor& color, const std::wstring& font, const float& fontSize)
{
	m_content = content;
	m_color = color;
	m_font = font;
	m_fontSize = fontSize;
	InitializeFormat();
	InitializeColor();
	InitializeLayout();
}

UIText::~UIText()
{
}

void UIText::Render()
{
	__super::Render();

	if (!m_layout) return;

	TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
	if (!transform) return;

	// 텍스트 정렬에 맞춘 pivot 보정
	DWRITE_TEXT_METRICS metrics;
	m_layout->GetMetrics(&metrics);
	D2D1_POINT_2F pivotOffset = {
		metrics.width * 0.5f, // 예시로 중앙 정렬 가정
		metrics.height * 0.5f
	};
	D2D1::Matrix3x2F pivotAdjust = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Scale(transform->GetScale().x, transform->GetScale().y) *
		D2D1::Matrix3x2F::Rotation(transform->GetRotation() * 180.0f / Define::PI) *
		D2D1::Matrix3x2F::Translation(transform->GetPosition().x, transform->GetPosition().y);

	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();

	d2dDeviceContext->SetTransform(pivotAdjust * mat);

	d2dDeviceContext->DrawTextLayout(
		D2D1::Point2F(0, 0),
		m_layout.Get(),
		m_pBrush.Get()
	);
}

void UIText::SetTextAlignment(ETextFormat format)
{
	if (!m_dWriteTextFormat)
		return;

	switch (format)
	{
	case ETextFormat::TopLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::TopCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::TopRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::MiddleLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::MiddleCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::MiddleRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::BottomLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case ETextFormat::BottomCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case ETextFormat::BottomRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	default:
		break;
	}
}

void UIText::SetText(const std::wstring& content)
{
	m_content = content;
}

void UIText::SetText(const float& val)
{
	m_content = std::to_wstring(val);
}

void UIText::SetColor(const FColor& color)
{
	m_color = color;
}

void UIText::SetFontSize(const float& _size)
{
	m_fontSize = _size;
	InitializeFormat();
}

void UIText::InitializeFormat()
{
	IDWriteFactory* m_dWriteFactory = D2DRenderManager::GetInstance().m_dWriteFactory.Get();

	m_dWriteFactory->CreateTextFormat(
		m_font.c_str(), // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_fontSize,
		L"", //locale
		&m_dWriteTextFormat
	);

	// 텍스트를 수평 및 수직으로 중앙에 맞춥니다.
	m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void UIText::InitializeColor()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();
	m_pBrush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}

void UIText::InitializeLayout()
{
	m_layout = nullptr;
	HRESULT hr = D2DRenderManager::GetInstance().m_dWriteFactory->CreateTextLayout(
		m_content.c_str(),
		static_cast<UINT32>(m_content.length()),
		m_dWriteTextFormat.Get(),
		FLT_MAX, FLT_MAX,
		&m_layout
	);

	if (FAILED(hr)) return;
}
