#pragma once
#include <Component/UIComponent.h>

enum class ButtonState
{
	Idle,
	Hover,
	Pressed
};

class UIButton : public UIComponent
{
public:
	UIButton();
	virtual ~UIButton();

	void SetImages(const std::wstring& idle, const std::wstring& hover, const std::wstring& pressed)
	{
		m_idleImage = idle;
		m_hoverImage = hover;
		m_pressedImage = pressed;

		LoadData(m_idleImage); // 초기 이미지 세팅
	}

	void LoadData(const std::wstring& path);
	void SetScale(const FVector2& _scale) { scale = _scale; }
	void SetScale(const float& _x, const float& _y) { scale = FVector2(_x, _y); }
	void SetScale(const float& _x) { scale = FVector2(_x, _x); }
	void SetPosition(const FVector2& pos);

	void Update(const float& deltaSeconds) override;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}

private:
	UIComponent* m_parents = nullptr;
	std::vector<UIComponent*> m_child;

	ButtonState m_state = ButtonState::Idle;

	std::wstring m_idleImage;
	std::wstring m_hoverImage;
	std::wstring m_pressedImage;

	bool m_prevMouseDown = false;

	std::function<void()> onClick;

private:
	struct ButtonSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ButtonSlot> slots;
};

