#pragma once
#include "Component/RenderComponent.h"
#include <Math/TMath.h>
#include <Math/TColor.h>
#include <Math/Transform.h>
#include <Define/Define.h>
#include <sstream>

using namespace Define;
class TextRenderComponent : public RenderComponent
{
public:
	TextRenderComponent();
	TextRenderComponent(const std::wstring& content, const FColor& color, const std::wstring& font, const float& fontSize);
	~TextRenderComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	void Render() override;

	ComPtr<IDWriteTextLayout> m_layout;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	void InitializeFormat();
	void InitializeColor();
	void InitializeLayout();
	void SetTextAlignment(ETextFormat format);
	inline std::wstring& GetText() { return m_content; }
	void SetText(const std::wstring& content);
	void SetText(const float& val);
	template<typename... Args>
	void SetTextFormat(const Args&... args)
	{
		std::wostringstream oss;

		// C++14에서는 fold expression이 없으므로 초기화 리스트 트릭 사용
		using expander = int[];
		(void)expander {
			0, (static_cast<void>(oss << args), 0)...
		};

		m_content = oss.str();
		InitializeLayout();
	}
	void SetColor(const FColor& color);
	void SetFontSize(const float& _size);
	void SetPosition(const FVector2& pos);
	void SetScale(const FVector2& scale);
	void SetTransformType(const ETransformType& type);

public:
	ETransformType m_eTransformType = ETransformType::D2D;
	Transform m_transform;
	std::wstring m_content = L"";
	FColor m_color;
	std::wstring m_font = L"Consolas";
	float m_fontSize = 24.0f;
};

