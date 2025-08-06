#include "pch.h"
#include "TextRenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Object/Camera.h>
#include <System/RenderSystem.h>
#include <Math/TColor.h>
#include <Math/TMath.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Helpers/CoordHelper.h>
#include <Helpers/FileHelper.h>

TextRenderComponent::TextRenderComponent()
{
	m_color = FColor::Black;
	drawType = EDrawType::ScreenSpace;
	m_layer = 101;
}

TextRenderComponent::TextRenderComponent(const std::wstring& content = L"", const FColor& color = FColor::Black, const std::wstring& font = L"Consolas", const float& fontSize = 24.0f)
{
	drawType = EDrawType::ScreenSpace;
	m_content = content;
	m_color = color;
	m_font = font;
	m_fontSize = fontSize;
	InitializeFormat();
	InitializeColor();
	InitializeLayout();
}

TextRenderComponent::~TextRenderComponent()
{
}

void TextRenderComponent::Initialize()
{
	__super::Initialize();

	InitializeFormat();
	InitializeColor();
	InitializeLayout();

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostUpdateWork);
}

void TextRenderComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TextRenderComponent::Release()
{
	__super::Release();
}

void TextRenderComponent::Render()
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || m_content.empty()) return;
	__super::Render();
	
	InitializeLayout();

	D2D1_POINT_2F textPosition = D2D1::Point2F(0, 0); // destRect의 중앙 (0,0)을 기준점으로 사용
	context->DrawTextLayout(textPosition, m_layout.Get(), m_pBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
	//FVector2 relativeSize = GetRelativeSize();
	//D2D1_POINT_2F textPosition = D2D1::Point2F(-relativeSize.x / 2, -relativeSize.y / 2);
	//context->DrawTextLayout(textPosition, m_layout.Get(), m_pBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

}

float TextRenderComponent::GetBitmapSizeX()
{
	InitializeLayout();
	return m_metrics.width;
}

float TextRenderComponent::GetBitmapSizeY()
{
	InitializeLayout();
	return m_metrics.height;
}

FVector2 TextRenderComponent::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void TextRenderComponent::InitializeFormat()
{
	IDWriteFactory* m_dWriteFactory = D2DRenderManager::GetInstance().m_dWriteFactory.Get();

	if (m_eTextSource == ETextSource::System)
	{
		m_dWriteFactory->CreateTextFormat(
			m_font.c_str(), // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_fontSize,
			m_locale.c_str(), //locale
			&m_dWriteTextFormat
		);
	}
	else
	{
		ComPtr<IDWriteFontFile> fontFile;
		HRESULT hr = m_dWriteFactory->CreateFontFileReference(
			m_filePath.c_str(),
			nullptr,
			&fontFile
		);
		if (FAILED(hr)) {
			// 에러 처리
			return;
		}

		m_dWriteFactory->CreateTextFormat(
			m_font.c_str(), // 폰트 이름을 실제 폰트 파일 이름과 맞춰줘야 함
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_fontSize,
			m_locale.c_str(),
			&m_dWriteTextFormat
		);
	}
}

void TextRenderComponent::InitializeColor()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();
	m_pBrush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}

void TextRenderComponent::InitializeLayout()
{
	if (m_metricsDirty) {
		m_layout = nullptr;
		HRESULT hr = D2DRenderManager::GetInstance().m_dWriteFactory->CreateTextLayout(
			m_content.c_str(),
			static_cast<UINT32>(m_content.length()),
			m_dWriteTextFormat.Get(),
			FLT_MAX, FLT_MAX,
			&m_layout
		);
		m_layout->GetMetrics(&m_metrics);
		m_metricsDirty = false;
		if (FAILED(hr)) return;
	}
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

	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetText(const std::wstring& content)
{
	m_content = content;
	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetText(const float& val)
{
	m_content = std::to_wstring(val);
	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetColor(const FColor& color)
{
	m_color = color;
	InitializeColor();
	m_metricsDirty = true;
}

void TextRenderComponent::SetFontSize(const float& _size)
{
	m_fontSize = _size;
	m_metricsDirty = true;
	InitializeFormat();
}

void TextRenderComponent::SetFont(const std::wstring& _fontName, const std::wstring& _fontLocale)
{
	m_font = _fontName;
	m_locale = _fontLocale;
	InitializeFormat();
}

void TextRenderComponent::SetFontFromFile(const std::wstring& filePath)
{
	m_filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + filePath);
	m_eTextSource = ETextSource::File;
}

void TextRenderComponent::SetIgnoreCameraTransform(bool bIgnore)
{
	bIgnoreCameraTransform = bIgnore;
}