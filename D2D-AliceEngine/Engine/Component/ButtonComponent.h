#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Define/Define.h>

class Transform;
class ButtonComponent : public RenderComponent
{
public:
	ButtonComponent();
	~ButtonComponent();

	virtual void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;
	virtual FVector2 GetRelativeSize() override;

	void LoadData(const std::wstring& path);

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}
	
private:
	// UI 영역 내 마우스 위치 확인 헬퍼 메서드
	bool IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize);
	
	struct ClickFunctionSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ClickFunctionSlot> slots;
	Define::EButtonState m_state = Define::EButtonState::Idle;
};