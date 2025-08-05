#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Define/Define.h>
#include <unordered_map>
#include <functional>

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

	void LoadData(Define::EButtonState state, const std::wstring& path);

	void ExecuteStateAction(Define::EButtonState state);
	void SetCurrentState(Define::EButtonState state);

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
	std::unordered_map<Define::EButtonState, std::shared_ptr<ID2D1Bitmap1>> m_bitmaps;

	// ���º� �Լ� ���� (������ ����)
	template<typename F>
	void SetStateAction(Define::EButtonState state, F&& action)
	{
		stateActionSlots[state] = { owner, action };
	}

	void SetActive(bool _active) { bActive = _active; }
	bool GetActive() const { return bActive; }
	
private:
	// UI ���� �� ���콺 ��ġ Ȯ�� ���� �޼���
	bool IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize);
	
	struct ClickFunctionSlot
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	Define::EButtonState m_state = Define::EButtonState::Idle;
	bool m_prevMouseDown = false;
	
	// ���º� �Լ� ������ ���� unordered_map (������ ����)
	std::unordered_map<Define::EButtonState, ClickFunctionSlot> stateActionSlots;

	bool bActive = true;
};