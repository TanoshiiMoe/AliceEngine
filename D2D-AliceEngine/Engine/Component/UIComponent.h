#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Math/Transform.h>
#include "InputComponent.h"

/*
*	UI ������Ʈ�Դϴ�.
*	�ؽ�Ʈ�� UIText(ī�޶� ����), ���� ���� Text�� �и�
*	���콺 Ŭ�� �̺�Ʈ ó���� ��ư������ ó���ϸ� �˴ϴ�.
*	�� �̹����� Ŭ���� �� �ְ� �Ѵٸ� ����ϼŵ� �Ǳ� �� ������...
*	�߰������� �̹����� �ִϸ��̼ǵ� ���� �� ������ ������Ʈ�� �����Ծ��
*/

class UIComponent : public RenderComponent
{
public:
	UIComponent();
	~UIComponent();

	virtual void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	virtual void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();

	Transform m_transform;

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;

	template<typename F>
	void SetAction(F&& action)
	{
		slots.push_back({ owner, action });
	}

private:
	UIComponent* m_parents = nullptr;
	std::vector<UIComponent*> m_child;

private:
	struct ClickSlot 
	{
		WeakObjectPtr<gameObject> weakPtr;
		std::function<void()> func;
	};
	std::vector<ClickSlot> slots;
};