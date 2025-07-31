#pragma once
#include <Component/UIComponent.h>
#include <Define/Define.h>
#include <Math/Transform.h>

using namespace Define;
class UIButton : public UIComponent
{
public:
	UIButton();
	virtual ~UIButton();

	void SetImages(const std::wstring& idle, const std::wstring& hover, const std::wstring& pressed);
	void LoadData(const std::wstring& path);
	inline void SetScale(const FVector2& _scale)
	{
		if (isHovered)
		{
			scale = m_hoveredScale;
		}
		else
			scale = _scale;
	}

	inline void SetScale(const float& _x, const float& _y)
	{
		if (isHovered)
		{
			scale = m_hoveredScale;
		}
		else
			scale = FVector2(_x, _y);
	}

	inline void SetScale(const float& _x)
	{
		if (isHovered)
		{
			scale = m_hoveredScale;
		}
		else
			scale = FVector2(_x, _x);
	}

	void SetPosition(const FVector2& pos);

	// Hovered 상태 사이즈를 세부 조절
	void SetHoverScale(const float& _x) { m_hoveredScale = FVector2(_x, _x); }

	void SetPivot(float _x, float _y);
	void SetAnchor(EUIScreenAnchor anchor, const FVector2& offset = FVector2(0, 0));
	void SetAnchor(EUIScreenAnchor anchor, const float& offsetX = 0, const float& offsetY = 0);

	EUIScreenAnchor GetAnchor() { return m_anchor; }
	EUIScreenAnchor m_anchor = EUIScreenAnchor::TopLeft;

	void Update(const float& deltaSeconds) override;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}

private:
	UIComponent* m_parents = nullptr;
	std::vector<UIComponent*> m_child;

	EButtonState m_state = EButtonState::Idle;

	FVector2 m_hoveredScale;

	std::wstring m_idleImage;
	std::wstring m_hoverImage;
	std::wstring m_pressedImage;

	bool m_prevMouseDown = false;
	bool isHovered = false;

	std::function<void()> onClick;

private:
	struct ButtonSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ButtonSlot> slots;
};

