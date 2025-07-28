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

TextRenderComponent::TextRenderComponent()
{
	m_color = FColor::Black;
	drawType = EDrawType::ScreenSpace;
	//InitializeFormat();
	//InitializeColor();
	//InitializeLayout();
}

TextRenderComponent::TextRenderComponent(const std::wstring& content = L"", const FColor& color = FColor::Black, const std::wstring& font = L"Consolas", const float& fontSize = 24.0f)
{
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

	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_PostUpdateWork,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
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
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Identity();

	InitializeLayout();
	// 피벗 보정
	D2D1_POINT_2F pivotOffset = {
		m_metrics.width * GetPivot()->x,
		m_metrics.height * GetPivot()->y
	};
	D2D1::Matrix3x2F pivotAdjust = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

	// 로컬 변환 먼저 적용 (내부 transform + pivotAdjust)
	D2D1::Matrix3x2F localTransform =
		D2D1::Matrix3x2F::Scale(m_transform.GetScale().x, m_transform.GetScale().y) *
		D2D1::Matrix3x2F::Rotation(m_transform.GetRotation()) *
		D2D1::Matrix3x2F::Translation(m_transform.GetPosition().x, m_transform.GetPosition().y);

	// 그 뒤에 world, camera 적용
	// m_pTransform이 계산된 worldTransform의 주소를 가지고 있음
	if (m_eTransformType == ETransformType::Unity)
	{
		D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
		D2D1::Matrix3x2F world = GetTransform() ? GetTransform()->ToMatrix() : D2D1::Matrix3x2F::Identity();

		Camera* camera = SceneManager::GetCamera();
		D2D1::Matrix3x2F cameraInv = camera ? camera->m_transform->ToMatrix() : D2D1::Matrix3x2F::Identity();
		cameraInv.Invert();

		view = unity * world * cameraInv;
		view = view * unity * D2D1::Matrix3x2F::Translation(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	}
	else
	{
		view = GetTransform() ? GetTransform()->ToMatrix() : D2D1::Matrix3x2F::Identity();
	}

	D2D1::Matrix3x2F finalTransform = pivotAdjust * localTransform * view;
	context->SetTransform(finalTransform);

	// 그리기
	context->DrawText(
		m_content.c_str(),
		static_cast<UINT32>(m_content.length()),
		m_dWriteTextFormat.Get(),
		D2D1::RectF(0, 0, m_metrics.width, m_metrics.height),
		m_pBrush.Get()
	);
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

void TextRenderComponent::InitializeFormat()
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
}

void TextRenderComponent::SetText(const std::wstring& content)
{
	m_content = content;
	m_metricsDirty = true;
}

void TextRenderComponent::SetText(const float& val)
{
	m_content = std::to_wstring(val);
	m_metricsDirty = true;
}

void TextRenderComponent::SetColor(const FColor& color)
{
	m_color = color;
	m_metricsDirty = true;
}

void TextRenderComponent::SetFontSize(const float& _size)
{
	m_fontSize = _size;
	m_metricsDirty = true;
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
