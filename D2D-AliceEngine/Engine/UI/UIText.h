#pragma once
#include <Component/UIComponent.h>
#include <Object/gameObject.h>
#include <Math/TMath.h>
#include <Math/TColor.h>
#include <Math/Transform.h>
#include <Define/Define.h>
#include <sstream>

using namespace Define;
class gameObject;
class UIText : public UIComponent
{
public:
	UIText();
	UIText(const std::wstring& content, const FColor& color, const std::wstring& font, const float& fontSize);
	virtual ~UIText();

	void Initialize() override;

	void Render() override;

	void SetTextAlignment(ETextFormat format);
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
	void SetScale(const FVector2& _scale) { scale = _scale; }
	void SetTransformType(const ETransformType& type);

	ComPtr<IDWriteTextLayout> m_layout;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

private:
	void InitializeFormat();
	void InitializeColor();
	void InitializeLayout();

private:
	std::wstring m_content = L"";
	std::wstring m_font = L"Consolas";
	float m_fontSize = 24.0f;
	FColor m_color;

	DWRITE_TEXT_METRICS m_metrics{};
	ETransformType m_eTransformType = ETransformType::D2D;
	bool m_metricsDirty = true;
};