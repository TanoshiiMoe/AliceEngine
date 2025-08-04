#pragma once
#include <Component/RenderComponent.h>
#include <Core/ObjectHandler.h>
#include <Math/Transform.h>
#include "InputComponent.h"
#include <Define/Define.h>

/*
*	UI ������Ʈ�Դϴ�.
*	�ؽ�Ʈ�� UIText(ī�޶� ����), ���� ���� Text�� �и�
*	���콺 Ŭ�� �̺�Ʈ ó���� ��ư������ ó���ϸ� �˴ϴ�.
*	�� �̹����� Ŭ���� �� �ְ� �Ѵٸ� ����ϼŵ� �Ǳ� �� ������...
*	�߰������� �̹����� �ִϸ��̼ǵ� ���� �� ������ ������Ʈ�� �����Ծ��
*/

using namespace Define;
class gameObject;
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

	virtual void SetPosition(const FVector2& pos);
	virtual void SetPivot(float _x, float _y);
	virtual void SetAnchor(EUIScreenAnchor anchor, const FVector2& offset = FVector2(0, 0));
	virtual void SetAnchor(EUIScreenAnchor anchor, const float& offsetX, const float& offsetY);

	EUIScreenAnchor GetAnchor() { return m_anchor; }
	EUIScreenAnchor m_anchor = EUIScreenAnchor::TopLeft;

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