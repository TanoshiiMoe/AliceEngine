#pragma once
#include "../pch.h"
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"
#include <Math/TColor.h>
#include <Math/TMath.h>
#include <Math/Transform.h>

using namespace Define;
class TextRenderComponent : public RenderComponent
{
public:
	TextRenderComponent();
	TextRenderComponent(const std::wstring& content, const FColor& color, const std::wstring& font, const float& fontSize);
	~TextRenderComponent();

	void Initialize() override;
	void Update() override;
	void Release() override;

	void Render();

	ComPtr<IDWriteTextLayout> m_layout;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	void InitializeFormat();
	void InitializeColor();
	void InitializeLayout();
	void SetTextAlignment(ETextFormat format);
	inline std::wstring& GetText() { return m_content; }
	void SetText(const std::wstring& content);
	void SetColor(const FColor& color);
	void SetFontSize(const float& _size);
	void SetPosition(const FVector2& pos);
	void SetScale(const FVector2& scale);
	void SetTransformType(const ETransformType& type);

public:
	ETransformType m_eTransformType = ETransformType::D2D;
	Transform m_transform;
	std::wstring m_content = L"";
	FColor m_color = FColor::Black;
	std::wstring m_font = L"Consolas";
	float m_fontSize = 24.0f;
};

