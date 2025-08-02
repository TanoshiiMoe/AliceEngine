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

	void LoadData(const std::wstring& path);

	FVector2 GetSize();

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}
	
private:
	struct ClickFunctionSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ClickFunctionSlot> slots;
	EButtonState m_state = EButtonState::Idle;
};