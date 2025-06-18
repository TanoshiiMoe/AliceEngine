#include "pch.h"
#include "TextRenderComponent.h"
#include <Manager/D2DRenderManager.h>

TextRenderComponent::TextRenderComponent()
{
	InitializeFormat();
	InitializeColor();
}

TextRenderComponent::TextRenderComponent(const std::wstring& content = L"", const FColor& color = FColor::Black, const std::wstring& font = L"Consolas", const float& fontSize = 24.0f)
{
	m_content = content;
	m_color = color;
	m_font = font;
	m_fontSize = fontSize;
	InitializeFormat();
	InitializeColor();
}

TextRenderComponent::~TextRenderComponent()
{
}

void TextRenderComponent::Initialize()
{

}

void TextRenderComponent::Update()
{
}

void TextRenderComponent::Release()
{
}

void TextRenderComponent::Render()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::Get().m_d2dDeviceContext.Get();
	D2D1_SIZE_F size = d2dDeviceContext->GetSize();
	
	D2D1_MATRIX_3X2_F _transform = D2D1::Matrix3x2F::Scale(m_transform.GetScale().x, m_transform.GetScale().y) *
		D2D1::Matrix3x2F::Rotation(m_transform.GetRotation()) *
		D2D1::Matrix3x2F::Translation(m_transform.GetPosition().x, m_transform.GetPosition().y);  // 이동

	// 기준점
	d2dDeviceContext->SetTransform(_transform);

	d2dDeviceContext->DrawText(
		m_content.c_str(),
		static_cast<UINT32>(m_content.length()),
		m_dWriteTextFormat.Get(),
		D2D1::RectF(0, 0, size.width, size.height / 2),
		m_pBrush.Get()
	);
}

void TextRenderComponent::InitializeFormat()
{
	IDWriteFactory* m_dWriteFactory = D2DRenderManager::Get().m_dWriteFactory.Get();

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
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void TextRenderComponent::InitializeColor()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::Get().m_d2dDeviceContext.Get();
	m_pBrush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}

void TextRenderComponent::SetContent(const std::wstring& content)
{
	m_content = content;
}

void TextRenderComponent::SetColor(const FColor& color)
{
	m_color = color;
	InitializeColor();
}

void TextRenderComponent::SetFontSize(const float& _size)
{
	m_font = _size;
	InitializeFormat();
}

void TextRenderComponent::SetPosition(const FVector2& pos)
{
	m_transform.SetPosition(pos.x, pos.y);
}