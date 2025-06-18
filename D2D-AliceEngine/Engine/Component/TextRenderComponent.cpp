#include "pch.h"
#include "TextRenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Object/Camera.h>
#include <Manager/SceneManager.h>

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
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();  // 이동

	if (m_eTransformType == ETransformType::Unity)
	{
		Camera* camera = SceneManager::GetCamera();
		D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
		D2D1::Matrix3x2F world = m_pTransform->ToMatrix();
		D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();
		view = view * unity;
		cameraInv.Invert();
		view = view * world * cameraInv;
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	D2D1_SIZE_F size = context->GetSize();

	D2D1::Matrix3x2F _transform = D2D1::Matrix3x2F::Scale(m_transform.GetScale().x, m_transform.GetScale().y) *
		D2D1::Matrix3x2F::Rotation(m_transform.GetRotation()) *
		D2D1::Matrix3x2F::Translation(m_transform.GetPosition().x, m_transform.GetPosition().y);  // 이동

	view = _transform * view;
	context->SetTransform(view);

	context->DrawText(
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
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void TextRenderComponent::InitializeColor()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::Get().m_d2dDeviceContext.Get();
	m_pBrush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}

void TextRenderComponent::SetTextAlignment(ETextFormat format)
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

void TextRenderComponent::SetScale(const FVector2& scale)
{
	m_transform.SetScale(scale.x, scale.y);
}

void TextRenderComponent::SetTransformType(const ETransformType& type)
{
	m_eTransformType = type;
}
