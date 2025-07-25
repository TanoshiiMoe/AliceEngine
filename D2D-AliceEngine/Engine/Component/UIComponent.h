#pragma once
#include <Component/RenderComponent.h>
#include <Manager/UIManager.h>

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

	virtual void Update() override {}
	virtual void Update(const float& deltaSeconds) override {}
	virtual void Render() override {}

	virtual float GetSizeX() { return 0.0f; }
	virtual float GetSizeY() { return 0.0f; }

	virtual void OnMouseEvent(const POINT& mousePos, bool isClick) {}
};

